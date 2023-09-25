#include <vin/modules/window/windowmodule.h>
#include <vin/core/error/error.h>

#ifdef VIN_LINUX
    #ifdef VIN_USE_WAYLAND
        #define GLFW_EXPOSE_NATIVE_WAYLAND
        #define GLFW_EXPOSE_NATIVE_EGL
        #include <wayland-egl.h>
        #include <GLFW/glfw3native.h>

#include <utility>
    #elif VIN_USE_X11
        #define GLFW_EXPOSE_NATIVE_X11
        #include <GLFW/glfw3native.h>
    #endif
#elif VIN_WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
#endif

Vin::Module::WindowModule::WindowModule() = default;
Vin::Module::WindowModule::WindowModule(int width, int height, Core::String title)
    : width{ width }, height{ height }, title{ std::move( title) } {}

void Vin::Module::WindowModule::Initialize() {
    ASSERT(glfwInit(), "Couldn't initialize GLFW.")

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);

    glfwSetWindowSizeCallback(window, GlfwWindowSizeCallback);
    glfwSetFramebufferSizeCallback(window, GlfwWindowFrameBufferSizeCallback);

    ASSERT(window, "Couldn't create window.")
}

void Vin::Module::WindowModule::Uninitialize() {
    glfwDestroyWindow(window);
}

void* Vin::Module::WindowModule::GetNativeWindowHandle() {
#ifdef VIN_LINUX
    #ifdef VIN_USE_WAYLAND

    wl_surface* surface = glfwGetWaylandWindow(window);
    ASSERT(surface, "Couldn't get wayland surface.")

    return wl_egl_window_create(surface, width, height);

    #elif VIN_USE_X11

    return glfwGetX11Window(window);

    #endif
#elif VIN_WIN32

    return glfwGetWin32Window(window);

#endif
}

void* Vin::Module::WindowModule::GetNativeDisplayHandle() {
#ifdef VIN_LINUX
    #ifdef VIN_USE_WAYLAND

    return glfwGetWaylandDisplay();

    #elif VIN_USE_X11

    return glfwGetX11Display();

    #endif
#elif VIN_WIN32

    return glfwGetWin32Display();

#endif
}

void Vin::Module::WindowModule::GetWindowSize(int& width, int& height) {
    width = this->width;
    height = this->height;
}

void Vin::Module::WindowModule::GetFrameBufferSize(int& width, int& height) {
    glfwGetFramebufferSize(window, &width, &height);
}

void Vin::Module::GlfwWindowSizeCallback(GLFWwindow *window, int width, int height) {
    WindowModule* windowModule = (WindowModule*)glfwGetWindowUserPointer(window);
    windowModule->width = width;
    windowModule->height = height;
    windowModule->onWindowResize.Invoke(width, height);
}

void Vin::Module::GlfwWindowFrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    WindowModule* windowModule = (WindowModule*)glfwGetWindowUserPointer(window);
    windowModule->onFrameBufferResize.Invoke(width, height);
}
