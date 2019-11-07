#include "Command.h"

void InputState::Update() {
	StateUpdate();
	TriggerUpdate();

}

void InputState::StateUpdate() {
	//キー入力保存情報更新
	m_InputState = 0;
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W) || CDirectInput::GetInstance().GetStickUporRight(CDirectInput::GamePadStick::LeftY)) {
		m_InputState += 1 << m_keyStateBitPoint.Wkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S) || CDirectInput::GetInstance().GetStickDownorLeft(CDirectInput::GamePadStick::LeftY)) {
		m_InputState += 1 << m_keyStateBitPoint.Skey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D) || CDirectInput::GetInstance().GetStickUporRight(CDirectInput::GamePadStick::LeftX)) {
		m_InputState += 1 << m_keyStateBitPoint.Dkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A) || CDirectInput::GetInstance().GetStickDownorLeft(CDirectInput::GamePadStick::LeftX)) {
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
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RSHIFT)) {
		m_InputState += 1 << m_keyStateBitPoint.RSHIFT;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LSHIFT)) {
		m_InputState += 1 << m_keyStateBitPoint.LSHIFT;
	}
}

void InputState::TriggerUpdate() {
	//キー入力保存情報更新
	m_InputStateTrigger = 0;

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP)) {
		m_InputStateTrigger += 1 << m_keyStateBitPoint.Upkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN)) {
		m_InputStateTrigger += 1 << m_keyStateBitPoint.Downkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RIGHT)) {
		m_InputStateTrigger += 1 << m_keyStateBitPoint.Rightkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LEFT)) {
		m_InputStateTrigger += 1 << m_keyStateBitPoint.Leftkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
		m_InputStateTrigger += 1 << m_keyStateBitPoint.Spacekey;
	}
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_E)) {
		m_InputStateTrigger += 1 << m_keyStateBitPoint.Ekey;
	}
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_Q)) {
		m_InputStateTrigger += 1 << m_keyStateBitPoint.Qkey;
	}
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RSHIFT)) {
		m_InputStateTrigger += 1 << m_keyStateBitPoint.RSHIFT;
	}
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LSHIFT)) {
		m_InputStateTrigger += 1 << m_keyStateBitPoint.LSHIFT;
	}
}

bool InputState::getInput(int key) {
	return (m_InputState & key);
}

bool InputState::getInputTrigger(int key) {
	return (m_InputStateTrigger & key);
}