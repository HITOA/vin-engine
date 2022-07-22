#include "window.hpp"

#include <logger.hpp>

Vin::Window::Window()
{
	glfwSetErrorCallback(Window::GlfwErrorCallback);

	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = std::unique_ptr<GLFWwindow, GlfwWindowDeleter>{
		glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL)
	};

	glfwMakeContextCurrent(window.get());
}

void Vin::Window::SetWindowSize(int width, int height)
{
	wWidth = width;
	wHeight = height;

	if (window)
		glfwSetWindowSize(window.get(), width, height);
}

int Vin::Window::GetWindowWidth()
{
	return wWidth;
}

int Vin::Window::GetWindowHeight()
{
	return wHeight;
}

void Vin::Window::SetWindowTitle(const char* title)
{
	wTitle = title;

	if (window)
		glfwSetWindowTitle(window.get(), title);
}

const char* Vin::Window::GetWindowTitle()
{
	return wTitle;
}

void Vin::Window::SwapBuffer()
{
	glfwSwapBuffers(window.get());
}

bool Vin::Window::ShouldClose()
{
	return glfwWindowShouldClose(window.get());
}

void Vin::Window::GlfwErrorCallback(int errcode, const char* description)
{
	Logger::Err("Glfw error {} : {}", errcode, description);
}
