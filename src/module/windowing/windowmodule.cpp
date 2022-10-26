#include "windowmodule.hpp"

#include "core/assert.hpp"
#include <GLFW/glfw3.h>

#include "assets/asset.hpp"
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
	handler.Bind(WindowResizeEvent{ (uint32_t)width, (uint32_t)height });
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

	case GLFW_KEY_ESCAPE: return Vin::Key::Escape;
	case GLFW_KEY_ENTER: return Vin::Key::Enter;
	case GLFW_KEY_TAB: return Vin::Key::Tab;
	case GLFW_KEY_BACKSPACE: return Vin::Key::BackSpace;
	case GLFW_KEY_INSERT: return Vin::Key::Insert;
	case GLFW_KEY_DELETE: return Vin::Key::Delete;
	case GLFW_KEY_RIGHT: return Vin::Key::Right;
	case GLFW_KEY_LEFT: return Vin::Key::Left;
	case GLFW_KEY_DOWN: return Vin::Key::Down;
	case GLFW_KEY_UP: return Vin::Key::Up;
	case GLFW_KEY_PAGE_UP: return Vin::Key::PageUp;
	case GLFW_KEY_PAGE_DOWN: return Vin::Key::PageDown;
	case GLFW_KEY_HOME: return Vin::Key::Home;
	case GLFW_KEY_END: return Vin::Key::End;
	case GLFW_KEY_CAPS_LOCK: return Vin::Key::CapsLock;
	case GLFW_KEY_SCROLL_LOCK: return Vin::Key::ScrollLock;
	case GLFW_KEY_NUM_LOCK: return Vin::Key::NumLock;
	case GLFW_KEY_PRINT_SCREEN: return Vin::Key::PrintScreen;
	case GLFW_KEY_PAUSE: return Vin::Key::Pause;

	case GLFW_KEY_F1: return Vin::Key::Key_F1;
	case GLFW_KEY_F2: return Vin::Key::Key_F2;
	case GLFW_KEY_F3: return Vin::Key::Key_F3;
	case GLFW_KEY_F4: return Vin::Key::Key_F4;
	case GLFW_KEY_F5: return Vin::Key::Key_F5;
	case GLFW_KEY_F6: return Vin::Key::Key_F6;
	case GLFW_KEY_F7: return Vin::Key::Key_F7;
	case GLFW_KEY_F8: return Vin::Key::Key_F8;
	case GLFW_KEY_F9: return Vin::Key::Key_F9;
	case GLFW_KEY_F10: return Vin::Key::Key_F10;
	case GLFW_KEY_F11: return Vin::Key::Key_F11;
	case GLFW_KEY_F12: return Vin::Key::Key_F12;
	case GLFW_KEY_F13: return Vin::Key::Key_F13;
	case GLFW_KEY_F14: return Vin::Key::Key_F14;
	case GLFW_KEY_F15: return Vin::Key::Key_F15;
	case GLFW_KEY_F16: return Vin::Key::Key_F16;
	case GLFW_KEY_F17: return Vin::Key::Key_F17;
	case GLFW_KEY_F18: return Vin::Key::Key_F18;
	case GLFW_KEY_F19: return Vin::Key::Key_F19;
	case GLFW_KEY_F20: return Vin::Key::Key_F20;
	case GLFW_KEY_F21: return Vin::Key::Key_F21;
	case GLFW_KEY_F22: return Vin::Key::Key_F22;
	case GLFW_KEY_F23: return Vin::Key::Key_F23;
	case GLFW_KEY_F24: return Vin::Key::Key_F24;
	case GLFW_KEY_F25: return Vin::Key::Key_F25;

	case GLFW_KEY_KP_0: return Vin::Key::Key_NP0;
	case GLFW_KEY_KP_1: return Vin::Key::Key_NP1;
	case GLFW_KEY_KP_2: return Vin::Key::Key_NP2;
	case GLFW_KEY_KP_3: return Vin::Key::Key_NP3;
	case GLFW_KEY_KP_4: return Vin::Key::Key_NP4;
	case GLFW_KEY_KP_5: return Vin::Key::Key_NP5;
	case GLFW_KEY_KP_6: return Vin::Key::Key_NP6;
	case GLFW_KEY_KP_7: return Vin::Key::Key_NP7;
	case GLFW_KEY_KP_8: return Vin::Key::Key_NP8;
	case GLFW_KEY_KP_9: return Vin::Key::Key_NP9;

	case GLFW_KEY_KP_DECIMAL: return Vin::Key::NPDecimal;
	case GLFW_KEY_KP_DIVIDE: return Vin::Key::NPDivide;
	case GLFW_KEY_KP_MULTIPLY: return Vin::Key::NPMultiply;
	case GLFW_KEY_KP_SUBTRACT: return Vin::Key::NPSubtract;
	case GLFW_KEY_KP_ADD: return Vin::Key::NPAdd;
	case GLFW_KEY_KP_ENTER: return Vin::Key::NPEnter;
	case GLFW_KEY_KP_EQUAL: return Vin::Key::NPEqual;
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

	m_Window = glfwCreateWindow((int)m_Info->width, (int)m_Info->height, m_Info->title, nullptr, nullptr);

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
	Input::Update();
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
