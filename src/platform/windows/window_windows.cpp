#include "window_windows.hpp"

#ifdef VIN_PLATFORM_WINDOWS

#include "core/events/applicationevent.hpp"
#include "core/events/inputevent.hpp"

#include "core/assert.hpp"

static bool _isGlfwInit = false;

static void GlfwErrorCallback(int err, const char* desc)
{
	printf("GLFW ERR %i: %s\n", err, desc);
}

void Vin::WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	WindowsWindow* w = (WindowsWindow*)glfwGetWindowUserPointer(window);
	Event e{ EVENT(EventType::WindowResize, EventCategoryApplication) };
	e.CreateData<WindowResizeInfo>(width, height);
	w->Dispatch(e);
}

void Vin::WindowCloseCallback(GLFWwindow* window)
{
	WindowsWindow* w = (WindowsWindow*)glfwGetWindowUserPointer(window);
	Event e{ EVENT(EventType::WindowClose, EventCategoryApplication) };
	w->Dispatch(e);
}

void Vin::WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	WindowsWindow* w = (WindowsWindow*)glfwGetWindowUserPointer(window);

	switch (action)
	{
	case GLFW_PRESS: {
		Event e{ EVENT(EventType::KeyPressed, EventCategoryInput | EventCategoryKeyboard) };
		e.CreateData<WindowKeyInfo>(key, scancode, mods);
		w->Dispatch(e);
		break;
	}
	case GLFW_RELEASE: {
		Event e{ EVENT(EventType::KeyReleased, EventCategoryInput | EventCategoryKeyboard) };
		e.CreateData<WindowKeyInfo>(key, scancode, mods);
		w->Dispatch(e);
		break;
	}
	case GLFW_REPEAT: {
		Event e{ EVENT(EventType::KeyHold, EventCategoryInput | EventCategoryKeyboard) };
		e.CreateData<WindowKeyInfo>(key, scancode, mods);
		w->Dispatch(e);
		break;
	}
	}
}

void Vin::WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	WindowsWindow* w = (WindowsWindow*)glfwGetWindowUserPointer(window);
	
	switch (action)
	{
	case GLFW_PRESS: {
		Event e{ EVENT(EventType::MouseButtonPressed, EventCategoryInput | EventCategoryMouseButton) };
		e.CreateData<WindowMouseButtonInfo>(button, mods);
		w->Dispatch(e);
		break;
	}
	case GLFW_REPEAT: {
		Event e{ EVENT(EventType::MouseButtonReleased, EventCategoryInput | EventCategoryMouseButton) };
		e.CreateData<WindowMouseButtonInfo>(button, mods);
		w->Dispatch(e);
		break;
	}
	}
}

void Vin::WindowMouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	WindowsWindow* w = (WindowsWindow*)glfwGetWindowUserPointer(window);
	Event e{ EVENT(EventType::MouseScrolled, EventCategoryInput | EventCategoryMouse) };
	e.CreateData<WindowMouseScrollInfo>(xOffset, yOffset);
	w->Dispatch(e);
}

void Vin::WindowMouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
{
	WindowsWindow* w = (WindowsWindow*)glfwGetWindowUserPointer(window);
	Event e{ EVENT(EventType::MouseMoved, EventCategoryInput | EventCategoryMouse) };
	e.CreateData<WindowMouseMoveInfo>(xPos, yPos);
	w->Dispatch(e);
}


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
	m_Context->SwapBuffer();
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

	if (!_isGlfwInit) {
		int status = glfwInit();
		VIN_ASSERT(status == GLFW_TRUE, "Glfw failed initialization.");

		glfwSetErrorCallback(GlfwErrorCallback);
	}

	m_Window = glfwCreateWindow(info.width, info.height, info.title, nullptr, nullptr);
	glfwSetWindowUserPointer(m_Window, this);

	m_Context = GraphicsContext::Create(m_Window);
	m_Context->Init();

	glfwSetWindowSizeCallback(m_Window, WindowResizeCallback);
	glfwSetWindowCloseCallback(m_Window, WindowCloseCallback);
	glfwSetKeyCallback(m_Window, WindowKeyCallback);
	glfwSetMouseButtonCallback(m_Window, WindowMouseButtonCallback);
	glfwSetScrollCallback(m_Window, WindowMouseScrollCallback);
	glfwSetCursorPosCallback(m_Window, WindowMouseMoveCallback);
}

void Vin::WindowsWindow::Terminate()
{
	glfwDestroyWindow(m_Window);
}

std::unique_ptr<Vin::Window> Vin::CreateWindow(const WindowInfo& info)
{
	return std::make_unique<WindowsWindow>(info);
}

#endif // VIN_PLATFORM_WINDOWS