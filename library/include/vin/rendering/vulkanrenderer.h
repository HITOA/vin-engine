#ifndef VIN_ENGINE_VULKANRENDERER_H
#define VIN_ENGINE_VULKANRENDERER_H

#include <vin/rendering/renderer.h>
#include <thread>
#include <vin/core/templates/concurrentcircularqueue.h>
#include <vulkan/vulkan.h>
#include <vin/core/memory/tlsfallocator.h>
#include <functional>
#include <atomic>
#include <vin/core/templates/fixedvector.h>

#if defined(VIN_WIN32)
    #define VK_USE_PLATFORM_WIN32_KHR
    #include <vulkan/vulkan_win32.h>
    #define KHR_SURFACE_EXTENSION_NAME  VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(VIN_LINUX)
    #if defined(VIN_USE_WAYLAND)
        #define VK_USE_PLATFORM_WAYLAND_KHR
        #include <vulkan/vulkan_wayland.h>
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

namespace Vin {

    struct VulkanRenderThreadCommand {
        enum Enum {
            CreateVulkanContext,
            DestroyVulkanContextAndExitThread,
            Present
        };

        VulkanRenderThreadCommand::Enum type{};
        void* data{ nullptr };
        std::function<void(void*)> freer{ nullptr };
    };

    class VulkanRenderer : public Renderer {
    public:
        bool Init(const RendererInitInfo& initInfo) final;
        void Terminate() final;
        void Present() final;

    private:
        bool CheckLayerSupport(const char* layerName);
        bool GetQueueFamily(VkPhysicalDevice device, uint32_t queueFlags, uint32_t* queueFamily);
        bool GetDevices(const char** extensions, uint32_t extensionCount);

        void CmdCreateVulkanContext(VulkanRenderThreadCommand& cmd);
        void CmdDestroyVulkanContext(VulkanRenderThreadCommand& cmd);
        void CmdPresent(VulkanRenderThreadCommand& cmd);

        void RenderThread();

    private:
        enum ThreadStatus {
            NotInitialized,
            Starting,
            Ok,
            FailedVulkanInitialization
        };

        ConcurrentCircularQueue<VulkanRenderThreadCommand, 512> renderThreadCommand{};
        std::thread renderThread{};
        std::atomic<ThreadStatus> renderThreadStatus{ NotInitialized };
        Vin::Core::TLSFAllocator<1 << 16> allocator{};

        VkInstance instance{};
        FixedVector<VkPhysicalDevice, 32> physicalDevices;
        uint32_t physicalDeviceIdx{ 0 };
        VkDevice device{ VK_NULL_HANDLE };

        float mainQueuePriority{ 1.0f };
        uint32_t mainQueueFamily{};
        VkQueue mainQueue{};

        VkSurfaceKHR surface{};
    };

}

#endif //VIN_ENGINE_VULKANRENDERER_H
