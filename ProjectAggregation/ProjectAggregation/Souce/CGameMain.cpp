#include "CGameMain.h"
#include "DX11util.h"
#include "CCamera.h"
#include "CDirectInput.h"
#include "WindowSetting.h"
#include "definer.h"
#include "DX11Settransform.h"


//-----------------------------------------------
//ゲームメイン処理クラス
//-----------------------------------------------

bool CGameMain::Init(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen) {
	bool		sts;

	// DX11初期処理
	sts = DX11Init(hwnd, width, height, fullscreen);
	if (!sts) {
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}

	// DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(hinst, hwnd, width, height);

	testModel->Load("assets/model/EditStage.fbx");

	// プロジェクション変換行列初期化
	XMFLOAT3 eye = { 0,5,-5 };				// 視点
	XMFLOAT3 lookat = { 0,0,0 };			// 注視点
	XMFLOAT3 up = { 0,1,0 };					// 上向きベクトル

	CCamera::GetInstance()->Init(1.0f, 10000.0f, XM_PIDIV2, SCREEN_X, SCREEN_Y, eye, lookat, up);

	// 平行光源初期化
	DX11LightInit(DirectX::XMFLOAT4(1, 1, -1, 0));		// 平行光源の方向をセット

	//testModel->LoadFbxAnimation("assets/model/animation/dousakakuninn_ver01.fbx");
	
	return true;
}

void CGameMain::Main() {
	Input();

	Update();

	Render();
}

void CGameMain::Input() {
	CDirectInput::GetInstance().GetKeyBuffer();//キー入力取得
}

void CGameMain::Update() {

}

void CGameMain::Render() {
	// ターゲットバッファクリア	
	float ClearColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	
	// レンダリング前処理
	DX11BeforeRender(ClearColor);

	XMFLOAT4X4 mat;

	// ビュー変換行列セット
	mat = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mat);

	// プロジェクション変換行列セット
	mat = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mat);

	testModel->Draw();

	// レンダリング後処理
	DX11AfterRender();
}

void CGameMain::Exit() {
	DX11SetTransform::GetInstance()->Uninit();	DX11Uninit();
	DX11Uninit();
}