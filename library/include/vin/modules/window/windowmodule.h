#ifndef VIN_ENGINE_WINDOWMODULE_H
#define VIN_ENGINE_WINDOWMODULE_H

#include <vin/application/module.h>
#include <vin/core/templates/stdcontainers.h>
#include <vin/core/templates/event.h>
#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace Vin::Modules {

    void GlfwWindowSizeCallback(GLFWwindow* window, int width, int height);
    void GlfwWindowFrameBufferSizeCallback(GLFWwindow* window, int width, int height);

    class WindowModule : public Module {
    public:
        Event<void(int, int)> onWindowResize{};
        Event<void(int, int)> onFrameBufferResize{};

    public:
        WindowModule();
        WindowModule(int width, int height, String title);

        void Initialize() final;
        void Uninitialize() final;

        void EarlyUpdate(TimeStep) final;

        void* GetNativeWindowHandle();
        void* GetNativeDisplayHandle();
        GLFWwindow* GetGlfwWindow();

        void GetWindowSize(int& width, int& height);
        void GetFrameBufferSize(int& width, int& height);

    private:
        int width{ 640 }, height{ 480 };
        String title{ "Application" };
        GLFWwindow* window{ nullptr };

        friend void GlfwWindowSizeCallback(GLFWwindow* window, int width, int height);
        friend void GlfwWindowFrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    };

}

#endif //VIN_ENGINE_WINDOWMODULE_H
