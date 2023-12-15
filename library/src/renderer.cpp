#include <vin/rendering/renderer.h>

#include <vin/core/error/error.h>
#include <thread>
#include <vin/core/templates/concurrentcircularqueue.h>
#include <vulkan/vulkan.h>
#include <vin/core/memory/tlsfallocator.h>
#include <functional>
#include <atomic>
#include <vin/core/templates/fixedvector.h>
#include <vin/core/templates/scratch.h>

#if defined(VIN_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
    #include <vulkan/vulkan_win32.h>
    #define KHR_SURFACE_EXTENSION_NAME  VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(VIN_LINUX)
#if defined(VIN_USE_WAYLAND)
#define VK_USE_PLATFORM_WAYLAND_KHR
#include <vulkan/vulkan_wayland.h>
#include <wayland-egl-backend.h>
#define KHR_SURFACE_EXTENSION_NAME VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
#else
#define VK_USE_PLATFORM_XLIB_KHR
        #define VK_USE_PLATFORM_XCB_KHR
        #include <vulkan/vulkan_xcb.h>
        #define KHR_SURFACE_EXTENSION_NAME VK_KHR_XCB_SURFACE_EXTENSION_NAME
#endif
#elif defined(VIN_OSX)
#define VK_USE_PLATFORM_MACOS_MVK
    #include <vulkan/vulkan_macos.h>
    #define KHR_SURFACE_EXTENSION_NAME  VK_MVK_MACOS_SURFACE_EXTENSION_NAME
#endif

#define VIN_VULKAN_MAX_SHADER_COUNT 256
#define VIN_VULKAN_MAX_PROGRAM_COUNT 256
#define VIN_VULKAN_MAX_FRAMEBUFFER_COUNT 256

struct RendererCommand {
    enum Enum {
        Terminate,
        Initialize,
        Reset,
        LoadShader
    };

    RendererCommand::Enum type{};
    void* data{ nullptr };
    std::function<void(void*)> freer{ nullptr };
};

struct ShaderLoadingInfo {
    char* data{ nullptr };
    size_t size{ 0 };
    uint32_t idx{};
};

struct Framebuffer {
    VkImage images[8];
    VkFramebuffer framebuffer{ VK_NULL_HANDLE };
    VkRenderPass renderPass{};
};

struct Shader {
    VkShaderModule shaderModule{ VK_NULL_HANDLE };
    char entrypointName[256]{};
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
};

struct Program {
    uint32_t shaders[8]{};
    VkPipelineLayout pipelineLayout{};
};

struct RendererState {
    std::thread rendererThread{};
    Vin::ConcurrentCircularQueue<RendererCommand, 512> commandQueue{};
    Vin::Core::TLSFAllocator<1 << 16> allocator{};

    uint32_t flags{ 0 };

#ifndef VIN_BUILD_NDEBUG
    bool validationLayerEnabled{ false };
    VkDebugUtilsMessengerEXT debugMessenger{};
#endif

    VkInstance instance{ VK_NULL_HANDLE };

    VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
    VkDevice device{ VK_NULL_HANDLE };

    float mainQueuePriority{ 1.0f };
    uint32_t mainQueueFamily{};
    VkQueue mainQueue{};

    VkSurfaceKHR surface{ VK_NULL_HANDLE };
    VkSwapchainKHR swapchain{ VK_NULL_HANDLE };
    VkSurfaceFormatKHR swapchainFormat{};
    VkExtent2D swapchainExtent{};
    uint32_t swapchainImageCount{ 32 };
    VkImage swapchainImages[32]{};
    //VkImageView swapchainImageviews[32]{};

    Shader shaders[VIN_VULKAN_MAX_SHADER_COUNT]{};
    Program programs[VIN_VULKAN_MAX_PROGRAM_COUNT]{};
    Framebuffer framebuffers[VIN_VULKAN_MAX_FRAMEBUFFER_COUNT]{};

    //Cached VkPipeline?, will be flushed
};

bool stateInitialized{ false };
static RendererState state{};
auto freer = [](void* ptr){ state.allocator.Deallocate(ptr); };

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            Vin::Logger::Err(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            Vin::Logger::Warn(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            Vin::Logger::Log(pCallbackData->pMessage);
            break;
        default:
            Vin::Logger::Log(pCallbackData->pMessage);
            break;
    }

    return VK_FALSE;
}

bool CheckLayerSupport(const char *layerName) {
    static uint32_t supportedLayerCount{ 64 };
    static VkLayerProperties supportedLayers[64]{};
    if (supportedLayerCount == 64)
        vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers);

    for (uint32_t i = 0; i < supportedLayerCount; ++i) {
        if (strcmp(layerName, supportedLayers[i].layerName) == 0)
            return true;
    }

    return false;
}

bool GetQueueFamily(VkPhysicalDevice device, uint32_t queueFlags, uint32_t* queueFamily) {
    uint32_t queueFamilyCount{ 64 };
    VkQueueFamilyProperties queueFamilies[64]{};

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    for (uint32_t i = 0; i < queueFamilyCount; ++i) {
        if (queueFamilies[i].queueFlags & queueFlags) {;
            if (queueFamily)
                *queueFamily = i;
            return true;
        }
    }

    return false;
}

bool GetDevices(const char** extensions, uint32_t extensionCount) {
    uint32_t deviceCount{ 32 };
    VkPhysicalDevice devices[32]{};
    vkEnumeratePhysicalDevices(state.instance, &deviceCount, devices);

    for (uint32_t i = 0; i < deviceCount; ++i) {
        uint32_t deviceExtensionCount{ 256 };
        VkExtensionProperties deviceExtensions[256]{};

        vkEnumerateDeviceExtensionProperties(devices[i], nullptr, &deviceExtensionCount, deviceExtensions);

        uint32_t supportedExtension{ 0 };
        for (uint32_t i = 0; i < deviceExtensionCount; ++i) {
            for (uint32_t j = 0; j < extensionCount; ++j) {
                if (strcmp(deviceExtensions[i].extensionName, extensions[j]) == 0) {
                    ++supportedExtension;
                    break;
                }
            }
        }

        if (supportedExtension < extensionCount)
            continue;

        if (!GetQueueFamily(devices[i], VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT, nullptr))
            continue;

        state.physicalDevice = devices[i];
        return true;
    }

    return false;
}

bool CreateSurface(void *window, void *display) {

#if defined(VIN_LINUX)
#if defined(VIN_USE_WAYLAND)
    VkWaylandSurfaceCreateInfoKHR surfaceCreateInfo{};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.surface = (wl_surface*)((wl_egl_window*)window)->surface;
    surfaceCreateInfo.display = (wl_display*)display;

    return vkCreateWaylandSurfaceKHR(state.instance, &surfaceCreateInfo, nullptr, &state.surface) == VK_SUCCESS;
#endif
#endif

}

void CreateSwapchain() {
    VkSurfaceCapabilitiesKHR capabilities{};
    uint32_t formatCount{ 64 };
    VkSurfaceFormatKHR formats[64];
    uint32_t presentModeCount{ 8 };
    VkPresentModeKHR presentModes[8];

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(state.physicalDevice, state.surface, &capabilities);
    vkGetPhysicalDeviceSurfaceFormatsKHR(state.physicalDevice, state.surface, &formatCount, formats);
    vkGetPhysicalDeviceSurfacePresentModesKHR(state.physicalDevice, state.surface, &presentModeCount, presentModes);

    uint32_t formatIdx{ 0 };
    for (uint32_t i = 0; i < formatCount; ++i) {
        if (formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            formatIdx = i;
            break;
        }
    }

    uint32_t presentModeIdx{ 0 };
    for (uint32_t i = 0; i < presentModeCount; ++i) {
        if (state.flags & VIN_RENDERER_CONFIG_FLAG_MAILBOX_SWAPCHAIN) {
            if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
                presentModeIdx = i;
                break;
            }
        }
        if (presentModes[i] == VK_PRESENT_MODE_FIFO_KHR)
            presentModeIdx = i;
    }

    state.swapchainExtent.width = std::clamp(state.swapchainExtent.width,
                                               capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    state.swapchainExtent.height = std::clamp(state.swapchainExtent.height,
                                               capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && capabilities.maxImageCount < imageCount)
        imageCount = capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR sci{};
    sci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    sci.surface = state.surface;
    sci.minImageCount = imageCount;
    sci.imageFormat = formats[formatIdx].format;
    sci.imageColorSpace = formats[formatIdx].colorSpace;
    sci.imageExtent = state.swapchainExtent;
    sci.imageArrayLayers = 1;
    sci.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    sci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    sci.preTransform = capabilities.currentTransform;
    sci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    sci.presentMode = presentModes[presentModeIdx];
    sci.clipped = VK_TRUE;
    sci.oldSwapchain = state.swapchain;

    ASSERT(vkCreateSwapchainKHR(state.device, &sci, nullptr, &state.swapchain) == VK_SUCCESS, "Can't create swapchain.");

    vkGetSwapchainImagesKHR(state.device, state.swapchain, &state.swapchainImageCount, state.swapchainImages);
    state.swapchainFormat = formats[formatIdx];

    /*for (uint32_t i = 0; i < state.swapchainImageCount; ++i) {
        VkImageViewCreateInfo ivci{};
        ivci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        ivci.image = state.swapchainImages[i];
        ivci.viewType = VK_IMAGE_VIEW_TYPE_2D;
        ivci.format = state.swapchainFormat.format;
        ivci.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        ivci.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        ivci.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        ivci.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        ivci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        ivci.subresourceRange.baseMipLevel = 0;
        ivci.subresourceRange.levelCount = 1;
        ivci.subresourceRange.baseArrayLayer = 0;
        ivci.subresourceRange.layerCount = 1;

        ASSERT(vkCreateImageView(state.device, &ivci, nullptr, &state.swapchainImageviews[i]) == VK_SUCCESS, "Can't create imageview for swapchain.");
    }*/
}

//Destroy & Rebuild Swapchain, Framebuffer, Graphics Pipeline... (For surface format & size change)
void Reset() {
    //if (state.swapchain != VK_NULL_HANDLE)
    //    vkDestroySwapchainKHR(state.device, state.swapchain, nullptr);

    CreateSwapchain();
}

void Init(Vin::RendererInitInfo* initInfo) {
    uint32_t apiVersion{};
    vkEnumerateInstanceVersion(&apiVersion);

    VkApplicationInfo ai{};
    ai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    ai.pApplicationName = "Vin";
    ai.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    ai.pEngineName = "Vin";
    ai.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    ai.apiVersion = apiVersion;

    Vin::FixedVector<const char*, 64> requiredExtensions{
            VK_KHR_SURFACE_EXTENSION_NAME,
            KHR_SURFACE_EXTENSION_NAME
    };

    Vin::FixedVector<const char*, 64> requiredLayers{};

#ifndef VIN_BUILD_NDEBUG
    if (CheckLayerSupport("VK_LAYER_KHRONOS_validation")) {
        requiredLayers.TryPush("VK_LAYER_KHRONOS_validation");
        requiredExtensions.TryPush(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        state.validationLayerEnabled = true;
    }
    else
        Vin::Logger::Warn("Vulkan Validation Layer not found. (No debug information concerning vulkan will be printed)");
#endif

    VkInstanceCreateInfo ici{};
    ici.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ici.pNext = NULL;
    ici.flags = 0;
    ici.pApplicationInfo = &ai;

    ici.enabledLayerCount = requiredLayers.GetCount();
    ici.ppEnabledLayerNames = requiredLayers.GetData();

#ifdef VIN_OSX
    requiredExtensions.TryPush(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    ici.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    ici.enabledExtensionCount = requiredExtensions.GetCount();
    ici.ppEnabledExtensionNames = requiredExtensions.GetData();
    ASSERT(vkCreateInstance(&ici, nullptr, &state.instance) == VK_SUCCESS, "Can't create vulkan instance.");

#ifndef VIN_BUILD_NDEBUG
    if (state.validationLayerEnabled) {
        VkDebugUtilsMessengerCreateInfoEXT dumci{};
        dumci.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        dumci.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        dumci.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        dumci.pfnUserCallback = DebugCallback;
        dumci.pUserData = nullptr;

        auto fnCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(state.instance, "vkCreateDebugUtilsMessengerEXT");
        if (fnCreateDebugUtilsMessengerEXT != nullptr) {
            fnCreateDebugUtilsMessengerEXT(state.instance, &dumci, nullptr, &state.debugMessenger);
        }
    }
#endif

    ASSERT(CreateSurface(initInfo->nativeWindowHandle, initInfo->nativeDisplayHandle), "Can't create vulkan surface.");

    //Device
    Vin::FixedVector<const char*, 64> deviceExtensions{
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    ASSERT(GetDevices(deviceExtensions.GetData(), deviceExtensions.GetCount()), "Can't find suited GPU.");

    GetQueueFamily(state.physicalDevice, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT, &state.mainQueueFamily);

    VkDeviceQueueCreateInfo dqci{};
    dqci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    dqci.queueFamilyIndex = state.mainQueueFamily;
    dqci.queueCount = 1;
    dqci.pQueuePriorities = &state.mainQueuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo dci{};
    dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    dci.pQueueCreateInfos = &dqci;
    dci.queueCreateInfoCount = 1;
    dci.pEnabledFeatures = &deviceFeatures;

    dci.enabledExtensionCount = deviceExtensions.GetCount();
    dci.ppEnabledExtensionNames = deviceExtensions.GetData();

    dci.enabledLayerCount = requiredLayers.GetCount();
    dci.ppEnabledLayerNames = requiredLayers.GetData();

    ASSERT(vkCreateDevice(state.physicalDevice, &dci, nullptr, &state.device) == VK_SUCCESS, "Can't create logical device.");

    vkGetDeviceQueue(state.device, state.mainQueueFamily, 0, &state.mainQueue);

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(state.physicalDevice, state.mainQueueFamily, state.surface, &presentSupport);
    ASSERT(presentSupport, "Can't present to the given surface.");

    state.flags = initInfo->flags;
    state.swapchainExtent.width = initInfo->width;
    state.swapchainExtent.height = initInfo->height;

    memset(state.shaders, 0, sizeof(state.shaders));

    Reset();
}

void LoadShader(ShaderLoadingInfo* shaderLoadingInfo) {
    Vin::ScratchReader reader{ (uint8_t*)shaderLoadingInfo->data, shaderLoadingInfo->size };

    Shader* shader = &state.shaders[shaderLoadingInfo->idx];

    uint32_t stage = reader.Read<uint32_t>();
    uint32_t size = reader.Read<uint32_t>();

    VkShaderModuleCreateInfo shaderCreateInfo{};
    shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderCreateInfo.codeSize = size;
    shaderCreateInfo.pCode = (uint32_t*)reader.GetData();

    ASSERT(vkCreateShaderModule(state.device, &shaderCreateInfo, nullptr,
                                &shader->shaderModule) == VK_SUCCESS, "Can't create shader.");

    reader.Skip(size);

    shader->shaderStageInfo = VkPipelineShaderStageCreateInfo{};
    shader->shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader->shaderStageInfo.stage = (VkShaderStageFlagBits)stage;
    shader->shaderStageInfo.module = shader->shaderModule;
    shader->shaderStageInfo.pName = shader->entrypointName;
}

void Terminate() {
#ifndef VIN_BUILD_NDEBUG
    if (state.validationLayerEnabled) {
        auto fnDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(state.instance, "vkDestroyDebugUtilsMessengerEXT");
        if (fnDestroyDebugUtilsMessengerEXT != nullptr) {
            fnDestroyDebugUtilsMessengerEXT(state.instance, state.debugMessenger, nullptr);
        }
    }
#endif

    /*for (uint32_t i = 0; i < state.swapchainImageCount; ++i) {
        vkDestroyImageView(state.device, state.swapchainImageviews[i], nullptr);
    }*/

    vkDestroySwapchainKHR(state.device, state.swapchain, nullptr);
    vkDestroySurfaceKHR(state.instance, state.surface, nullptr);
    vkDestroyDevice(state.device, nullptr);
    vkDestroyInstance(state.instance, nullptr);
}

void RendererThreadMain() {
    bool run{ true };
    RendererCommand command;
    while (run) {
        while (state.commandQueue.IsEmpty()) {};
        if (state.commandQueue.TryPop(command) != Vin::ContainerStatus::Success)
            continue;

        switch (command.type) {
            case RendererCommand::Initialize:
                Init((Vin::RendererInitInfo*)command.data);
                break;
            case RendererCommand::Terminate:
                Terminate();
                run = false;
                break;
            case RendererCommand::LoadShader:
                LoadShader((ShaderLoadingInfo*)command.data);
                break;
            default:
                break;
        }

        if (command.freer)
            command.freer(command.data);
    }
}

void Vin::Renderer::Init(const RendererInitInfo& initInfo) {
    ASSERT(!stateInitialized, "Can't initialize the renderer two time.");
    state.rendererThread = std::thread{ RendererThreadMain };
    RendererCommand command{};
    command.type = RendererCommand::Initialize;
    command.data = state.allocator.Allocate(sizeof(RendererInitInfo));
    *(RendererInitInfo*)command.data = initInfo;
    command.freer = freer;
    state.commandQueue.TryPush(command);
    stateInitialized = true;
}

void Vin::Renderer::Terminate() {
    ASSERT(stateInitialized, "Terminate was called before Init().");
    RendererCommand command{};
    command.type = RendererCommand::Terminate;
    while (state.commandQueue.TryPush(command) != Vin::ContainerStatus::Success) {};
    state.rendererThread.join();
}

void Vin::Renderer::SyncRenderThread() {
    while (!state.commandQueue.IsEmpty())  {}
}

Vin::ShaderHandle Vin::Renderer::LoadShader(char* data, size_t size) {
    ShaderHandle handle = VIN_INVALID_HANDLE;
    for (uint32_t i = 0; i < VIN_VULKAN_MAX_SHADER_COUNT; ++i) {
        if (state.shaders[i].shaderModule == VK_NULL_HANDLE) {
            handle = i;
            state.shaders[i].shaderModule = (VkShaderModule)1;
            break;
        }
    }
    if (handle != VIN_INVALID_HANDLE) {
        RendererCommand command{};
        command.type = RendererCommand::LoadShader;
        command.data = state.allocator.Allocate(sizeof(ShaderLoadingInfo) + size);
        command.freer = freer;

        ShaderLoadingInfo* shaderLoadingInfo = (ShaderLoadingInfo*)command.data;
        shaderLoadingInfo->data = (char*)command.data + sizeof(ShaderLoadingInfo);
        shaderLoadingInfo->size = size;
        shaderLoadingInfo->idx = handle;

        memcpy(shaderLoadingInfo->data, data, size);

        if (state.commandQueue.TryPush(command) != Vin::ContainerStatus::Success) {
            command.freer(command.data);
            state.shaders[handle].shaderModule = (VkShaderModule)VK_NULL_HANDLE;
            return VIN_INVALID_HANDLE;
        }
    }
    return handle;
}