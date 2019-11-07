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
	const long GamePad_DeadZone = 20000;//入力無視範囲

	LPDIRECTINPUTDEVICE8	m_dikeyboard;	// キーボードデバイス
	LPDIRECTINPUTDEVICE8	m_dimouse;		// マウスデバイス
	char					m_keybuffer[256];		// キーボードバッファ
	char					m_oldkeybuffer[256];	// 前回の入力キーボードバッファ
	DIMOUSESTATE2			m_diMouseState;		// マウスの状態
	int						m_MousePosX;		// マウスのＸ座標
	int						m_MousePosY;		// マウスのＹ座標
	int						m_width;			// マウスのＸ座標最大
	int						m_height;			// マウスのＹ座標最大
	long					m_JoypadDI_X;		// ジョイパッドX座標
	long					m_JoypadDI_Y;		// ジョイパッドY座標
	DIJOYSTATE				m_JoySticksState;
	DIJOYSTATE				m_JoySticksTrigger;

	DIDEVCAPS m_diDevCaps;

	CDirectInput() :m_dinput(nullptr), m_dikeyboard(nullptr), m_dimouse(nullptr), m_MousePosX(0), m_MousePosY(0){
	}
public:
	LPDIRECTINPUT8			m_dinput;
	LPDIRECTINPUTDEVICE8	m_dijoypad = nullptr;		// ジョイパッド

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
	// DirectInput 初期処理
	//
	//		P1 : インスタンス値
	//		P2 : ウインドウハンドル値
	//
	//	戻り値
	//		true : 初期化成功
	//		false : 初期化失敗
	//----------------------------------
	bool Init(HINSTANCE hInst,HWND hwnd,int width,int height){
		HRESULT	hr;
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_dinput, NULL);
		if(FAILED(hr)) {
			return false;
		}

		// キーボードデバイス生成
		m_dinput->CreateDevice(GUID_SysKeyboard, &m_dikeyboard, NULL);
		if(FAILED(hr)) {
			return false;
		}

		// データフォーマットの設定
		hr = m_dikeyboard->SetDataFormat(&c_dfDIKeyboard);
		if(FAILED(hr)) {
			return false;
		}
		
		// 協調レベルの設定
		hr = m_dikeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if(FAILED(hr)) {
			return false;
		}

		// マウスデバイス生成
		m_dinput->CreateDevice(GUID_SysMouse, &m_dimouse, NULL);
		if(FAILED(hr)) {
			return false;
		}

		// データフォーマットの設定
		hr = m_dimouse->SetDataFormat(&c_dfDIMouse2);
		if(FAILED(hr)) {
			return false;
		}
		
		// 協調レベルの設定
		hr = m_dimouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if(FAILED(hr)) {
			return false;
		}

		SetGamePad(hInst, hwnd);

		// デバイスの設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;							// 相対値モード
		m_dimouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);		// 軸モードの設定


		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = 0;
		diprg.lMax = width - 1;

		m_dimouse->SetProperty(DIPROP_RANGE, &diprg.diph);		// Ｘ方向の範囲を指定
		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = 0;
		diprg.lMax = height - 1;
		m_dimouse->SetProperty(DIPROP_RANGE, &diprg.diph);	// Ｙ方向の範囲を指定

		m_height = height;
		m_width  = width;

		return true;
	}

	bool SetGamePad(HINSTANCE hInst, HWND hwnd) {
		HRESULT hr;
		// ジョイパッドの設定
		hr = m_dinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr) || m_dijoypad == nullptr) {
			return false;
		}

		// データフォーマットの設定
		hr = m_dijoypad->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hr)) {
			return false;
		}

		// 協調レベルの設定
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
	// マウス状態取得処理
	//----------------------------------
	void GetMouseState(){
		HRESULT	hr;
		// デバイスの認識
		hr = m_dimouse->Acquire();

		hr = m_dimouse->GetDeviceState(sizeof(m_diMouseState),&m_diMouseState);
		if(hr == DIERR_INPUTLOST){
			// デバイスの認識
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
	// マウスＸ座標取得処理
	//----------------------------------
	int GetMousePosX() const{
		return m_MousePosX;
	}

	//----------------------------------
	// マウスＹ座標取得処理
	//----------------------------------
	int GetMousePosY() const{
		return m_MousePosY;
	}

	//----------------------------------
	// マウス左ボタンチェック
	//----------------------------------
	bool GetMouseLButtonCheck() const{
		if(m_diMouseState.rgbButtons[0] & 0x80){
			return true;
		}else{
			return false;	
		}
	}

	//----------------------------------
	// マウス右ボタンチェック
	//----------------------------------
	bool GetMouseRButtonCheck() const{
		if(m_diMouseState.rgbButtons[1] & 0x80){
			return true;
		}else{
			return false;	
		}
	}

	//----------------------------------
	// マウス中央ボタンチェック
	//----------------------------------
	bool GetMouseCButtonCheck() const{
		if(m_diMouseState.rgbButtons[2] & 0x80){
			return true;
		}else{
			return false;	
		}
	}

	//----------------------------------
	// キーボードバッファ取得処理
	//----------------------------------
	void GetKeyBuffer(){
		HRESULT	hr;
		// デバイスの認識
		hr = m_dikeyboard->Acquire();
		// 前回の状態を保存
		memcpy(&m_oldkeybuffer,m_keybuffer,sizeof(m_keybuffer));
		hr = m_dikeyboard->GetDeviceState(sizeof(m_keybuffer),(LPVOID)&m_keybuffer);
		if(hr == DIERR_INPUTLOST){
			// デバイスの認識
			hr = m_dikeyboard->Acquire();
		}
	}

	//----------------------------------
	// キーが押されているかどうかをチェックする
	//		p1 :　チェックしたいキー番号
	//	戻り値
	//		true : 指定されたキーが押されている
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
	// キーが押されているかどうかをチェックする
	//		p1 :　チェックしたいキー番号(トリガー)
	//	戻り値
	//		true : 指定されたキーが押されている
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
	//ゲームパッド入力受け取り
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
	//ゲームパッド入力状態取得
	//==================================
	bool GetStickUporRight(GamePadStick DirectList) {
		//上入力か右入力を受け取ったらtrueを返す
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
		//下入力か左入力を受け取ったらtrueを返す
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
	// DirectInput 終了処理
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
