#include <vin/rendering/vulkanrenderer.h>
#include <vin/core/memory/memorymanager.h>
#include <vin/core/logger/logger.h>
#include <vin/core/templates/fixedvector.h>

bool Vin::VulkanRenderer::Init(const Vin::RendererInitInfo &initInfo) {
    if (renderThreadStatus == Ok) {
        Vin::Logger::Warn("Renderer already initalized. you can't initialized it twice.");
        return false;
    }

    renderThread = std::thread{ &Vin::VulkanRenderer::RenderThread, this };
    renderThreadStatus = Starting;

    VulkanRenderThreadCommand command{};
    command.type = VulkanRenderThreadCommand::CreateVulkanContext;
    command.data = allocator.Allocate(sizeof(RendererInitInfo));
    *((RendererInitInfo*)command.data) = initInfo;
    command.freer = [this](void* ptr) { allocator.Deallocate(ptr); };

    if (renderThreadCommand.TryPush(command) != ContainerStatus::Success)
        return false;
    return true;
}

void Vin::VulkanRenderer::Terminate() {
    if (renderThreadStatus == NotInitialized)
        return;
    VulkanRenderThreadCommand command{};
    command.type = VulkanRenderThreadCommand::DestroyVulkanContextAndExitThread;
    while (renderThreadCommand.TryPush(command) != ContainerStatus::Success) {};
    renderThread.join();
}

void Vin::VulkanRenderer::Present() {
    VulkanRenderThreadCommand command{};
    command.type = VulkanRenderThreadCommand::Present;
    renderThreadCommand.TryPush(command);
    while (!renderThreadCommand.IsEmpty()) {} //Wait
}

bool Vin::VulkanRenderer::CheckLayerSupport(const char *layerName) {
    static uint32_t supportedLayerCount{ 64 };
    static VkLayerProperties supportedLayers[64]{};
    if (supportedLayerCount == 64)
        vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers);

    for (uint32_t i = 0; i < supportedLayerCount; ++i) {
        Vin::Logger::Log((const char*)supportedLayers[i].layerName);
        if (strcmp(layerName, supportedLayers[i].layerName) == 0)
            return true;
    }

    return false;
}

bool Vin::VulkanRenderer::GetQueueFamily(VkPhysicalDevice device, uint32_t queueFlags, uint32_t* queueFamily) {
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

bool Vin::VulkanRenderer::GetDevices(const char** extensions, uint32_t extensionCount) {
    uint32_t deviceCount{ 32 };
    VkPhysicalDevice devices[32]{};
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

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

        physicalDevices.TryPush(devices[i]);
    }

    return physicalDevices.GetCount() > 0;
}

void Vin::VulkanRenderer::CmdCreateVulkanContext(Vin::VulkanRenderThreadCommand &cmd) {
    Vin::RendererInitInfo* initInfo = (Vin::RendererInitInfo*)cmd.data;

    uint32_t apiVersion{};
    vkEnumerateInstanceVersion(&apiVersion);

    VkApplicationInfo ai{};
    ai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    ai.pApplicationName = "Vin";
    ai.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    ai.pEngineName = "Vin";
    ai.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    ai.apiVersion = apiVersion;

    FixedVector<const char*, 64> requiredExtensions{
            VK_KHR_SURFACE_EXTENSION_NAME,
            KHR_SURFACE_EXTENSION_NAME
    };

    FixedVector<const char*, 64> requiredLayers{};

#ifndef VIN_BUILD_NDEBUG
    if (CheckLayerSupport("VK_LAYER_KHRONOS_validation"))
        requiredLayers.TryPush("VK_LAYER_KHRONOS_validation");
    else
        Vin::Logger::Warn("Vulkan Validation Layer not supported. (No debug information concerning vulkan will be printed)");
#endif

#ifdef VIN_USE_WAYLAND
    requiredExtensions.TryPush("VK_KHR_wayland_surface");
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

    if (vkCreateInstance(&ici, nullptr, &instance) != VK_SUCCESS) {
        Vin::Logger::Err("Couldn't create vulkan instance.");
        renderThreadStatus = FailedVulkanInitialization;
        return;
    }

    //Surface

#if defined(VIN_LINUX)
#if defined(VIN_USE_WAYLAND)
    VkWaylandSurfaceCreateInfoKHR surfaceCreateInfo{};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.surface = (wl_surface*)initInfo->nativeWindowHandle;
    surfaceCreateInfo.display = (wl_display*)initInfo->nativeDisplayHandle;

    if (vkCreateWaylandSurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
        Vin::Logger::Err("Couldn't create wayland surface.");
        renderThreadStatus = FailedVulkanInitialization;
        return;
    }
#endif
#endif

    //Device
    FixedVector<const char*, 64> deviceExtensions{
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    if (!GetDevices(deviceExtensions.GetData(), deviceExtensions.GetCount())) {
        Vin::Logger::Err("Couldn't find a suited device.");
        renderThreadStatus = FailedVulkanInitialization;
        return;
    }

    GetQueueFamily(physicalDevices[physicalDeviceIdx], VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT, &mainQueueFamily);

    VkDeviceQueueCreateInfo dqci{};
    dqci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    dqci.queueFamilyIndex = mainQueueFamily;
    dqci.queueCount = 1;
    dqci.pQueuePriorities = &mainQueuePriority;

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

    if (vkCreateDevice(physicalDevices[physicalDeviceIdx], &dci, nullptr, &device) != VK_SUCCESS) {
        Vin::Logger::Err("Couldn't create logical device.");
        renderThreadStatus = FailedVulkanInitialization;
        return;
    }

    vkGetDeviceQueue(device, mainQueueFamily, 0, &mainQueue);

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[physicalDeviceIdx], mainQueueFamily, surface, &presentSupport);
    if (!presentSupport) {
        Vin::Logger::Err("Can't present to the givent surface.");
        renderThreadStatus = FailedVulkanInitialization;
        return;
    }

    renderThreadStatus = Ok;
}

void Vin::VulkanRenderer::CmdDestroyVulkanContext(Vin::VulkanRenderThreadCommand &cmd) {
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
    renderThreadStatus = NotInitialized;
}

void Vin::VulkanRenderer::CmdPresent(Vin::VulkanRenderThreadCommand &cmd) {
    return;
}

void Vin::VulkanRenderer::RenderThread() {
    bool run{ true };
    while (run) {
        while (renderThreadCommand.IsEmpty()) {}; //Wait for command

        VulkanRenderThreadCommand command{};
        if (renderThreadCommand.TryPop(command) != ContainerStatus::Success)
            continue;

        switch (command.type) {
            case VulkanRenderThreadCommand::CreateVulkanContext: {
                CmdCreateVulkanContext(command);
                if (renderThreadStatus == FailedVulkanInitialization)
                    run = false; //Stop the thread if vulkan failed to initialize
                break;
            }
            case VulkanRenderThreadCommand::DestroyVulkanContextAndExitThread:
                CmdDestroyVulkanContext(command);
                run = false;
                break;
            case VulkanRenderThreadCommand::Present:
                CmdPresent(command);
                break;
            default:
                break;
        }

        if (command.freer)
            command.freer(command.data);
    }
}