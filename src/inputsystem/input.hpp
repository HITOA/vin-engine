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
		RightControl, RightShift
	};

	class Input {
	public:
		static void SetKeyState(Key key, KeyState state);
		static KeyState GetKeyState(Key key);

		static bool IsKeyDown(Key key);
		static bool IsKeyHold(Key key);
		static bool IsKeyUp(Key key);

		static void SetMousePosition(Vector2<int> position);
		static Vector2<int> GetMousePosition();
	private:
		static std::array<KeyState, 48> m_KeyStates;
		static Vector2<int> m_MousePosition;
	};

}
