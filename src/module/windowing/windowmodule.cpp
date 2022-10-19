#include "windowmodule.hpp"

#include "core/assert.hpp"
#include <GLFW/glfw3.h>

#include "assets/assetdatabase.hpp"
#include "inputsystem/input.hpp"

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

void Vin::WindowDropCallback(GLFWwindow* window, int count, const char** paths)
{
	WindowModule* winmod = (WindowModule*)glfwGetWindowUserPointer(window);
	EventHandler handler{};
	handler.Bind(WindowDropEvent{ count, paths });
	winmod->DispatchEvent(handler);
}

Vin::Key ParseGLFWKeyCode(int key) {
	switch (key) {
	case GLFW_KEY_SPACE: return Vin::Key::Space;
	case GLFW_KEY_APOSTROPHE: return Vin::Key::Apostrophe;
	case GLFW_KEY_COMMA: return Vin::Key::Comma;
	case GLFW_KEY_MINUS: return Vin::Key::Minus;
	case GLFW_KEY_PERIOD: return Vin::Key::Period;
	case GLFW_KEY_SLASH: return Vin::Key::Slash;
	case GLFW_KEY_0: return Vin::Key::Key_0;
	case GLFW_KEY_1: return Vin::Key::Key_1;
	case GLFW_KEY_2: return Vin::Key::Key_2;
	case GLFW_KEY_3: return Vin::Key::Key_3;
	case GLFW_KEY_4: return Vin::Key::Key_4;
	case GLFW_KEY_5: return Vin::Key::Key_5;
	case GLFW_KEY_6: return Vin::Key::Key_6;
	case GLFW_KEY_7: return Vin::Key::Key_7;
	case GLFW_KEY_8: return Vin::Key::Key_8;
	case GLFW_KEY_9: return Vin::Key::Key_9;
	case GLFW_KEY_SEMICOLON: return Vin::Key::Semicolon;
	case GLFW_KEY_EQUAL: return Vin::Key::Equal;
	case GLFW_KEY_A: return Vin::Key::Key_A;
	case GLFW_KEY_B: return Vin::Key::Key_B;
	case GLFW_KEY_C: return Vin::Key::Key_C;
	case GLFW_KEY_D: return Vin::Key::Key_D;
	case GLFW_KEY_E: return Vin::Key::Key_E;
	case GLFW_KEY_F: return Vin::Key::Key_F;
	case GLFW_KEY_G: return Vin::Key::Key_G;
	case GLFW_KEY_H: return Vin::Key::Key_H;
	case GLFW_KEY_I: return Vin::Key::Key_I;
	case GLFW_KEY_J: return Vin::Key::Key_J;
	case GLFW_KEY_K: return Vin::Key::Key_K;
	case GLFW_KEY_L: return Vin::Key::Key_L;
	case GLFW_KEY_M: return Vin::Key::Key_M;
	case GLFW_KEY_N: return Vin::Key::Key_N;
	case GLFW_KEY_O: return Vin::Key::Key_O;
	case GLFW_KEY_P: return Vin::Key::Key_P;
	case GLFW_KEY_Q: return Vin::Key::Key_Q;
	case GLFW_KEY_R: return Vin::Key::Key_R;
	case GLFW_KEY_S: return Vin::Key::Key_S;
	case GLFW_KEY_T: return Vin::Key::Key_T;
	case GLFW_KEY_U: return Vin::Key::Key_U;
	case GLFW_KEY_V: return Vin::Key::Key_V;
	case GLFW_KEY_W: return Vin::Key::Key_W;
	case GLFW_KEY_X: return Vin::Key::Key_X;
	case GLFW_KEY_Y: return Vin::Key::Key_Y;
	case GLFW_KEY_Z: return Vin::Key::Key_Z;
	case GLFW_KEY_LEFT_CONTROL: return Vin::Key::LeftControl;
	case GLFW_KEY_LEFT_SHIFT: return Vin::Key::LeftShift;
	case GLFW_KEY_RIGHT_CONTROL: return Vin::Key::RightControl;
	case GLFW_KEY_RIGHT_SHIFT: return Vin::Key::RightShift;
	default:
		return Vin::Key::UNKNOWN;
	}
}

void Vin::WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action) {
	case GLFW_PRESS:
		return Input::SetKeyState(ParseGLFWKeyCode(key), KeyState::KeyDown);
	case GLFW_REPEAT:
		return Input::SetKeyState(ParseGLFWKeyCode(key), KeyState::KeyHold);
	case GLFW_RELEASE:
		return Input::SetKeyState(ParseGLFWKeyCode(key), KeyState::KeyUp);
	default:
		return Input::SetKeyState(ParseGLFWKeyCode(key), KeyState::KeyUp);
	}
}

void Vin::WindowCursordPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	Vin::Input::SetMousePosition(Vin::Vector2<int>{(int)xpos, (int)ypos});
}

void Vin::WindowModule::Init()
{
	m_Info = AssetDatabase::AddAsset<WindowInfo>(WindowInfo{}, VIN_WINDOWINFO_ASSETPATH);

	if (!_isGlfwInit) {
		int status = glfwInit();
		VIN_ASSERT(status == GLFW_TRUE, "Glfw failed initialization.");

		glfwSetErrorCallback(GlfwErrorCallback);
	}

	m_Window = glfwCreateWindow(m_Info->width, m_Info->height, m_Info->title, nullptr, nullptr);

	glfwSetWindowUserPointer(m_Window, this);

	m_Context = GraphicsContext::Create(m_Window);
	m_Context->Init();

	glfwSetWindowSizeCallback(m_Window, WindowResizeCallback);
	glfwSetWindowCloseCallback(m_Window, WindowCloseCallback);
	glfwSetDropCallback(m_Window, WindowDropCallback);
	glfwSetKeyCallback(m_Window, WindowKeyCallback);
	glfwSetCursorPosCallback(m_Window, WindowCursordPosCallback);
}

void Vin::WindowModule::EarlyUpdate()
{
	glfwPollEvents();
}

void Vin::WindowModule::LateRender()
{
	m_Context->SwapBuffer();
}

void Vin::WindowModule::OnEvent(EventHandler handler) {
	if (WindowMouseState* event = handler.GetEvent<WindowMouseState>()) {
		switch (event->state) {
		case WindowMouseState::Normal:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		case WindowMouseState::Lock:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;
		default:
			break;
		}
	}
}
