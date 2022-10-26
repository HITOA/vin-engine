#pragma once

#include "vinpch.hpp"
#include "math/vector2.hpp"

namespace Vin {

	enum class KeyState {
		None = 0,
		KeyDown,
		KeyHold,
		KeyUp
	};

	enum class Key {
		UNKNOWN = -1,
		Space, Apostrophe, Comma, Minus, Period, Slash,
		Key_0, Key_1, Key_2, Key_3, Key_4, Key_5, Key_6,
		Key_7, Key_8, Key_9,
		Semicolon, Equal,
		Key_A, Key_B, Key_C, Key_D, Key_E, Key_F, Key_G,
		Key_H, Key_I, Key_J, Key_K, Key_L, Key_M, Key_N,
		Key_O, Key_P, Key_Q, Key_R, Key_S, Key_T, Key_U,
		Key_V, Key_W, Key_X, Key_Y, Key_Z,
		LeftControl, LeftShift,
		RightControl, RightShift,
		Escape, Enter, Tab, BackSpace, Insert, Delete,
		Right, Left, Down, Up,
		PageUp, PageDown, Home, End,
		CapsLock, ScrollLock, NumLock, PrintScreen, Pause,
		Key_F1, Key_F2, Key_F3, Key_F4, Key_F5, Key_F6,
		Key_F7, Key_F8, Key_F9, Key_F10, Key_F11, Key_F12,
		Key_F13, Key_F14, Key_F15, Key_F16, Key_F17, Key_F18,
		Key_F19, Key_F20, Key_F21, Key_F22, Key_F23, Key_F24,
		Key_F25,
		Key_NP0, Key_NP1, Key_NP2, Key_NP3, Key_NP4, Key_NP5, Key_NP6,
		Key_NP7, Key_NP8, Key_NP9,
		NPDecimal, NPDivide, NPMultiply, NPSubtract, NPAdd, NPEnter,
		NPEqual
	};

	class Input {
	public:
		static void SetKeyState(Key key, KeyState state);
		static KeyState GetKeyState(Key key);

		static bool IsKeyDown(Key key);
		static bool IsKeyHold(Key key);
		static bool IsKeyUp(Key key);
		static bool IsKeyPressed(Key key);

		static void SetMousePosition(Vector2<int> position);
		static Vector2<int> GetMousePosition();

		static void Update();
	private:
		static std::array<KeyState, 108> m_KeyStates;
		static Vector2<int> m_MousePosition;
	};

}
