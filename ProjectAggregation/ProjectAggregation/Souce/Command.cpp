#include "Command.h"

void InputState::Update() {
	m_InputState = 0;
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)) {
		m_InputState += 1 << m_keyStateBitPoint.Wkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S)) {
		m_InputState += 1 << m_keyStateBitPoint.Skey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D)) {
		m_InputState += 1 << m_keyStateBitPoint.Dkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {
		m_InputState += 1 << m_keyStateBitPoint.Akey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP)) {
		m_InputState += 1 << m_keyStateBitPoint.Upkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN)) {
		m_InputState += 1 << m_keyStateBitPoint.Downkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT)) {
		m_InputState += 1 << m_keyStateBitPoint.Rightkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT)) {
		m_InputState += 1 << m_keyStateBitPoint.Leftkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_SPACE)) {
		m_InputState += 1 << m_keyStateBitPoint.Spacekey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_E)) {
		m_InputState += 1 << m_keyStateBitPoint.Ekey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_Q)) {
		m_InputState += 1 << m_keyStateBitPoint.Qkey;
	}
}

bool InputState::getInput(int key) {
	return (m_InputState & key);
}