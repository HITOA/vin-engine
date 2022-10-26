#include "input.hpp"

std::array<Vin::KeyState, 108> Vin::Input::m_KeyStates{ Vin::KeyState::KeyUp };

Vin::Vector2<int> Vin::Input::m_MousePosition{ 0 };

void Vin::Input::SetKeyState(Key key, KeyState state) {
	if ((int)key < 0 || (int)key > m_KeyStates.size())
		return;
	m_KeyStates[(int)key] = state;
}
Vin::KeyState Vin::Input::GetKeyState(Key key) {
	if ((int)key < 0 || (int)key > m_KeyStates.size())
		return KeyState::None;
	return m_KeyStates[(int)key];
}

bool Vin::Input::IsKeyDown(Key key) {
	return GetKeyState(key) == KeyState::KeyDown && !(GetKeyState(key) == KeyState::KeyHold);
}

bool Vin::Input::IsKeyHold(Key key) {
	return GetKeyState(key) == KeyState::KeyHold;
}

bool Vin::Input::IsKeyUp(Key key) {
	return GetKeyState(key) == KeyState::KeyUp;
}

bool Vin::Input::IsKeyPressed(Key key)
{
	return GetKeyState(key) == KeyState::KeyDown || GetKeyState(key) == KeyState::KeyHold;
}

void Vin::Input::SetMousePosition(Vector2<int> position)
{
	m_MousePosition = position;
}

Vin::Vector2<int> Vin::Input::GetMousePosition()
{
	return m_MousePosition;
}

void Vin::Input::Update()
{
	for (KeyState& state : m_KeyStates)
		if (state == KeyState::KeyDown)
			state = KeyState::KeyHold;
}
