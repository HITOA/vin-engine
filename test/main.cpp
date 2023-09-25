#include <iostream>
#define GLFW_EXPOSE_NATIVE_WAYLAND
#define GLFW_EXPOSE_NATIVE_EGL
#include <wayland-egl.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <bgfx/bgfx.h>
#include <vin/core/error/error.h>
#include <typeindex>
#include <vin/core/templates/stdcontainers.h>
#include <vin/core/templates/event.h>

#include <vin/core/templates/ref.h>

#include <vin/vin.h>
#include <vin/modules/rendering/renderingmodule.h>


int main() {
    //setbuf(stdout, 0);

    Vin::App app{};
    app.AddModule<Vin::Module::WindowModule>();
    app.AddModule<Vin::Module::RenderingModule>();
    app.Run();

    /*ASSERT(1 != 1, "OWO ERROR")

    setbuf(stdout, 0);

    if (!glfwInit()) {
        std::cerr << "Couldn't initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);

    if (!window) {
        std::cerr << "Coudln't Create Window." << std::endl;
        glfwTerminate();
        return -1;
    }

    wl_surface* surface = glfwGetWaylandWindow(window);
    if (!surface) {
        std::cerr << "Can't get wayland surface" << std::endl;
        return -1;
    }

    int width{};
    int height{};
    glfwGetFramebufferSize(window, &width, &height);

    bgfx::Init init;
    init.platformData.nwh = wl_egl_window_create(surface, 640, 480);
    init.platformData.ndt = glfwGetWaylandDisplay();
    init.platformData.type = bgfx::NativeWindowHandleType::Wayland;
    //init.platformData.nwh = (void*)glfwGetX11Window(window);
    //init.platformData.ndt = glfwGetX11Display();
    init.resolution.width = 640;
    init.resolution.height = 480;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.type = bgfx::RendererType::Vulkan;
    init.vendorId = 0;
    bgfx::init(init);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glfwGetFramebufferSize(window, &width, &height);
        bgfx::reset(width, height, BGFX_RESET_VSYNC);
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, width, height);
        bgfx::touch(0);
        bgfx::frame();
    }

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();*/
}