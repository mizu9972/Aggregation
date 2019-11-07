#pragma once
#include	<dinput.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *PDIDOL, void *pContext);

class CDirectInput{
private:
	const long GamePad_DeadZone = 20000;//���͖����͈�

	LPDIRECTINPUTDEVICE8	m_dikeyboard;	// �L�[�{�[�h�f�o�C�X
	LPDIRECTINPUTDEVICE8	m_dimouse;		// �}�E�X�f�o�C�X
	char					m_keybuffer[256];		// �L�[�{�[�h�o�b�t�@
	char					m_oldkeybuffer[256];	// �O��̓��̓L�[�{�[�h�o�b�t�@
	DIMOUSESTATE2			m_diMouseState;		// �}�E�X�̏��
	int						m_MousePosX;		// �}�E�X�̂w���W
	int						m_MousePosY;		// �}�E�X�̂x���W
	int						m_width;			// �}�E�X�̂w���W�ő�
	int						m_height;			// �}�E�X�̂x���W�ő�
	long					m_JoypadDI_X;		// �W���C�p�b�hX���W
	long					m_JoypadDI_Y;		// �W���C�p�b�hY���W
	DIJOYSTATE				m_JoySticksState;
	DIJOYSTATE				m_JoySticksTrigger;

	DIDEVCAPS m_diDevCaps;

	CDirectInput() :m_dinput(nullptr), m_dikeyboard(nullptr), m_dimouse(nullptr), m_MousePosX(0), m_MousePosY(0){
	}
public:
	LPDIRECTINPUT8			m_dinput;
	LPDIRECTINPUTDEVICE8	m_dijoypad = nullptr;		// �W���C�p�b�h

	CDirectInput(const CDirectInput&) = delete;
	CDirectInput& operator=(const CDirectInput&) = delete;
	CDirectInput(CDirectInput&&) = delete;
	CDirectInput& operator=(CDirectInput&&) = delete;

	static CDirectInput& GetInstance(){
		static CDirectInput Instance;
		return Instance;
	}

	~CDirectInput(){
		Exit();
	}

	enum class GamePadStick {
		RightX,
		RightY,
		LeftX,
		LeftY,
	};


	//----------------------------------
	// DirectInput ��������
	//
	//		P1 : �C���X�^���X�l
	//		P2 : �E�C���h�E�n���h���l
	//
	//	�߂�l
	//		true : ����������
	//		false : ���������s
	//----------------------------------
	bool Init(HINSTANCE hInst,HWND hwnd,int width,int height){
		HRESULT	hr;
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_dinput, NULL);
		if(FAILED(hr)) {
			return false;
		}

		// �L�[�{�[�h�f�o�C�X����
		m_dinput->CreateDevice(GUID_SysKeyboard, &m_dikeyboard, NULL);
		if(FAILED(hr)) {
			return false;
		}

		// �f�[�^�t�H�[�}�b�g�̐ݒ�
		hr = m_dikeyboard->SetDataFormat(&c_dfDIKeyboard);
		if(FAILED(hr)) {
			return false;
		}
		
		// �������x���̐ݒ�
		hr = m_dikeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if(FAILED(hr)) {
			return false;
		}

		// �}�E�X�f�o�C�X����
		m_dinput->CreateDevice(GUID_SysMouse, &m_dimouse, NULL);
		if(FAILED(hr)) {
			return false;
		}

		// �f�[�^�t�H�[�}�b�g�̐ݒ�
		hr = m_dimouse->SetDataFormat(&c_dfDIMouse2);
		if(FAILED(hr)) {
			return false;
		}
		
		// �������x���̐ݒ�
		hr = m_dimouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if(FAILED(hr)) {
			return false;
		}

		SetGamePad(hInst, hwnd);

		// �f�o�C�X�̐ݒ�
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;							// ���Βl���[�h
		m_dimouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);		// �����[�h�̐ݒ�


		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = 0;
		diprg.lMax = width - 1;

		m_dimouse->SetProperty(DIPROP_RANGE, &diprg.diph);		// �w�����͈̔͂��w��
		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = 0;
		diprg.lMax = height - 1;
		m_dimouse->SetProperty(DIPROP_RANGE, &diprg.diph);	// �x�����͈̔͂��w��

		m_height = height;
		m_width  = width;

		return true;
	}

	bool SetGamePad(HINSTANCE hInst, HWND hwnd) {
		HRESULT hr;
		// �W���C�p�b�h�̐ݒ�
		hr = m_dinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr) || m_dijoypad == nullptr) {
			return false;
		}

		// �f�[�^�t�H�[�}�b�g�̐ݒ�
		hr = m_dijoypad->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hr)) {
			return false;
		}

		// �������x���̐ݒ�
		hr = m_dijoypad->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr)) {
			return false;
		}

		m_diDevCaps.dwSize = sizeof(DIDEVCAPS);
		hr = m_dijoypad->GetCapabilities(&m_diDevCaps);
		if (FAILED(hr)) {
			return false;
		}

		hr = m_dijoypad->EnumObjects(EnumAxesCallback, (VOID*)hwnd, DIDFT_AXIS);
		if (FAILED(hr)) {
			return false;
		}

		hr = m_dijoypad->Poll();
		if (FAILED(hr)) {
			hr = m_dijoypad->Acquire();
			while (hr == DIERR_INPUTLOST) {
				hr = m_dijoypad->Acquire();
			}
		}
		return true;
	}


	//----------------------------------
	// �}�E�X��Ԏ擾����
	//----------------------------------
	void GetMouseState(){
		HRESULT	hr;
		// �f�o�C�X�̔F��
		hr = m_dimouse->Acquire();

		hr = m_dimouse->GetDeviceState(sizeof(m_diMouseState),&m_diMouseState);
		if(hr == DIERR_INPUTLOST){
			// �f�o�C�X�̔F��
			hr = m_dimouse->Acquire();
		}

		m_MousePosX += m_diMouseState.lX;
		m_MousePosY += m_diMouseState.lY;
		if(m_MousePosX<0){
			m_MousePosX=0;
		}else if(m_MousePosX>m_width-1){
			m_MousePosX=m_width-1;
		}
		if(m_MousePosY<0){
			m_MousePosY=0;
		}else if(m_MousePosY>m_height-1){
			m_MousePosY=m_height-1;
		}
	}

	//----------------------------------
	// �}�E�X�w���W�擾����
	//----------------------------------
	int GetMousePosX() const{
		return m_MousePosX;
	}

	//----------------------------------
	// �}�E�X�x���W�擾����
	//----------------------------------
	int GetMousePosY() const{
		return m_MousePosY;
	}

	//----------------------------------
	// �}�E�X���{�^���`�F�b�N
	//----------------------------------
	bool GetMouseLButtonCheck() const{
		if(m_diMouseState.rgbButtons[0] & 0x80){
			return true;
		}else{
			return false;	
		}
	}

	//----------------------------------
	// �}�E�X�E�{�^���`�F�b�N
	//----------------------------------
	bool GetMouseRButtonCheck() const{
		if(m_diMouseState.rgbButtons[1] & 0x80){
			return true;
		}else{
			return false;	
		}
	}

	//----------------------------------
	// �}�E�X�����{�^���`�F�b�N
	//----------------------------------
	bool GetMouseCButtonCheck() const{
		if(m_diMouseState.rgbButtons[2] & 0x80){
			return true;
		}else{
			return false;	
		}
	}

	//----------------------------------
	// �L�[�{�[�h�o�b�t�@�擾����
	//----------------------------------
	void GetKeyBuffer(){
		HRESULT	hr;
		// �f�o�C�X�̔F��
		hr = m_dikeyboard->Acquire();
		// �O��̏�Ԃ�ۑ�
		memcpy(&m_oldkeybuffer,m_keybuffer,sizeof(m_keybuffer));
		hr = m_dikeyboard->GetDeviceState(sizeof(m_keybuffer),(LPVOID)&m_keybuffer);
		if(hr == DIERR_INPUTLOST){
			// �f�o�C�X�̔F��
			hr = m_dikeyboard->Acquire();
		}
	}

	//----------------------------------
	// �L�[��������Ă��邩�ǂ������`�F�b�N����
	//		p1 :�@�`�F�b�N�������L�[�ԍ�
	//	�߂�l
	//		true : �w�肳�ꂽ�L�[��������Ă���
	//----------------------------------
	bool CheckKeyBuffer(int keyno){
		if(m_keybuffer[keyno] & 0x80){
			return true;
		}
		else{
			return false;
		}
	}

	//----------------------------------
	// �L�[��������Ă��邩�ǂ������`�F�b�N����
	//		p1 :�@�`�F�b�N�������L�[�ԍ�(�g���K�[)
	//	�߂�l
	//		true : �w�肳�ꂽ�L�[��������Ă���
	//----------------------------------
	bool CheckKeyBufferTrigger(int keyno){
		if(((m_keybuffer[keyno]^m_oldkeybuffer[keyno]) & m_keybuffer[keyno]) & 0x80){
			return true;
		}
		else{
			return false;
		}
	}

	//----------------------------------
	//�Q�[���p�b�h���͎󂯎��
	//----------------------------------
	bool UpdateGamePad() {
		HRESULT hr;
		DIJOYSTATE OldJoyState;
		OldJoyState = m_JoySticksState;

		if (m_dijoypad == nullptr) {
			return false;
		}

		hr = m_dijoypad->Poll();
		if (FAILED(hr)) {
			hr = m_dijoypad->Acquire();
			while (hr == DIERR_INPUTLOST) {
				hr = m_dijoypad->Acquire();
			}
		}

		hr = m_dijoypad->GetDeviceState(sizeof(DIJOYSTATE), &m_JoySticksState);
		if (FAILED(hr)) {
			return false;
		}
		
		if (SUCCEEDED(hr)) {
			for (int cnt = 0; cnt < 32; cnt++) {
				m_JoySticksTrigger.rgbButtons[cnt] = ((OldJoyState.rgbButtons[cnt] ^ m_JoySticksState.rgbButtons[cnt]) &m_JoySticksState.rgbButtons[cnt]);
			}
		}
		else {
			m_dijoypad->Acquire();
		}

		return true;

	}

	//==================================
	//�Q�[���p�b�h���͏�Ԏ擾
	//==================================
	bool GetStickUporRight(GamePadStick DirectList) {
		//����͂��E���͂��󂯎������true��Ԃ�
		if (DirectList == GamePadStick::RightX || DirectList == GamePadStick::LeftX) {
			if (GetGamePadStick(DirectList) >= m_JoypadDI_X + GamePad_DeadZone) {
				return true;
			}
		}
		else if (DirectList == GamePadStick::RightY || DirectList == GamePadStick::LeftY) {
			if (GetGamePadStick(DirectList) >= m_JoypadDI_Y + GamePad_DeadZone) {
				return true;
			}
		}

		return false;
	}

	bool GetStickDownorLeft(GamePadStick DirectList) {
		//�����͂������͂��󂯎������true��Ԃ�
		if (DirectList == GamePadStick::RightX || DirectList == GamePadStick::LeftX) {
			if (GetGamePadStick(DirectList) <= m_JoypadDI_X - GamePad_DeadZone) {
				return true;
			}
		}
		else if (DirectList == GamePadStick::RightY || DirectList == GamePadStick::LeftY) {
			if (GetGamePadStick(DirectList) <= m_JoypadDI_Y - GamePad_DeadZone) {
				return true;
			}
		}

		return false;
	}

	long GetGamePadStick(GamePadStick DirectList) {
		if (m_dijoypad == nullptr) {
			return 0;
		}
		switch (DirectList) {
		case GamePadStick::RightX:
			return m_JoySticksState.lZ;
			break;

		case GamePadStick::RightY:
			return m_JoySticksState.lRz;
			break;

		case GamePadStick::LeftX:
			return m_JoySticksState.lX;
			break;

		case GamePadStick::LeftY:
			return m_JoySticksState.lY;
			break;

		}
	}
	bool GetGamePadPress(int key) {
		if (m_dijoypad == nullptr) {
			return false;
		}
		return ((m_JoySticksState.rgbButtons[key] & 0x80) != 0);
	}
	bool GetGamePadTrigger(int key) {
		if (m_dijoypad == nullptr) {
			return false;
		}
		return ((m_JoySticksTrigger.rgbButtons[key] & 0x80) != 0);
	}
	//----------------------------------
	// DirectInput �I������
	//----------------------------------
	void Exit(){
		if(m_dikeyboard!=nullptr){
			m_dikeyboard->Release();
		}
		if(m_dimouse!=nullptr){
			m_dimouse->Release();
		}
		if (m_dijoypad != nullptr) {
			m_dijoypad->Unacquire();
			m_dijoypad->Release();
		}

		if(m_dinput!=nullptr){
			m_dinput->Release();
		}
	}	
};
