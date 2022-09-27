#include "windowmodule.hpp"

#include <assert.hpp>
#include <GLFW/glfw3.h>

#include "assets/assetdatabase.hpp"

static bool _isGlfwInit = false;

static void GlfwErrorCallback(int err, const char* desc)
{
	printf("GLFW ERR %i: %s\n", err, desc);
}

void Vin::WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	WindowModule* winmod = (WindowModule*)glfwGetWindowUserPointer(window);
	winmod->m_Info->width = width;
	winmod->m_Info->height = height;
	EventHandler handler{};
	handler.Bind(WindowResizeEvent{ width, height });
	winmod->DispatchEvent(handler);
}

void Vin::WindowCloseCallback(GLFWwindow* window)
{
	WindowModule* winmod = (WindowModule*)glfwGetWindowUserPointer(window);
	EventHandler handler{};
	handler.Bind(WindowCloseEvent{ true });
	winmod->DispatchEvent(handler);
}

void Vin::WindowModule::Init()
{
	m_Info = AssetDatabase::AddAsset<WindowInfo>(WindowInfo{}, VIN_WINDOWINFO_ASSETID);

	if (!_isGlfwInit) {
		int status = glfwInit();
		VIN_ASSERT(status == GLFW_TRUE, "Glfw failed initialization.");

		glfwSetErrorCallback(GlfwErrorCallback);
	}

	m_Window = glfwCreateWindow(m_Info->width, m_Info->height, m_Info->title, nullptr, nullptr);

	AssetDatabase::AddAsset<GlfwWindowHolder>(GlfwWindowHolder{ m_Window }, VIN_GLFWWINDOW_ASSETID);

	glfwSetWindowUserPointer(m_Window, this);

	m_Context = GraphicsContext::Create(m_Window);
	m_Context->Init();

	glfwSetWindowSizeCallback(m_Window, WindowResizeCallback);
	glfwSetWindowCloseCallback(m_Window, WindowCloseCallback);
}

void Vin::WindowModule::EarlyUpdate()
{
	glfwPollEvents();
}

void Vin::WindowModule::Render()
{
	m_Context->SwapBuffer();
}
