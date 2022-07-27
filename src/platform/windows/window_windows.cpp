#include "window_windows.hpp"

#ifdef VIN_PLATFORM_WINDOWS

#include <gtest/gtest.h>

Vin::WindowsWindow::WindowsWindow(const WindowInfo& info)
{
	Init(info);
}

Vin::WindowsWindow::~WindowsWindow()
{
	Terminate();
}

void Vin::WindowsWindow::OnUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

const char* Vin::WindowsWindow::GetTitle() const
{
	return m_WindowData.title;
}

int Vin::WindowsWindow::GetWidth() const
{
	return m_WindowData.width;
}

int Vin::WindowsWindow::Getheight() const
{
	return m_WindowData.height;
}

void* Vin::WindowsWindow::GetNativeWindow() const
{
	return m_Window;
}

void Vin::WindowsWindow::Init(const WindowInfo& info)
{
	m_WindowData.title = info.title;
	m_WindowData.width = info.width;
	m_WindowData.height = info.height;

	int status = glfwInit();
	ASSERT_EQ(status, GLFW_TRUE) << "Glfw failed initialization";

	m_Window = glfwCreateWindow(info.width, info.height, info.title, nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_WindowData);
}

void Vin::WindowsWindow::Terminate()
{
	glfwDestroyWindow(m_Window);
}

Vin::Window* Vin::CreateWindow(const WindowInfo& info)
{
	return new WindowsWindow{ info };
}

void Vin::DestroyWindow(Window* window)
{
	delete window;
}

#endif // VIN_PLATFORM_WINDOWS