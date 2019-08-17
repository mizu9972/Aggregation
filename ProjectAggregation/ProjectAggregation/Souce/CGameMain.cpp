#include "CGameMain.h"
#include "DX11util.h"
#include "dx11mathutil.h"
#include "CCamera.h"
#include "CDirectInput.h"
#include "WindowSetting.h"
#include "definer.h"
#include "DX11Settransform.h"
#include "CScene.h"

//
#include "CModel.h"

CModel teststage;
XMFLOAT4X4 g_testmat;
//-----------------------------------------------
//ゲームメイン処理クラス
//-----------------------------------------------

bool CGameMain::Init(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen) {
	bool		sts;
	m_NowScene = new TitleScene;//タイトルシーンから
	// DX11初期処理
	sts = DX11Init(hwnd, width, height, fullscreen);
	if (!sts) {
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}

	// DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(hinst, hwnd, width, height);

	teststage.Init("assets/model/Ground.x.dat", "Shader/vs.fx", "Shader/MaterialColor_ps.fx");

	DX11MatrixIdentity(g_testmat);
	//testModel = new CFbxModel;
	//testModel->Load("assets/model/unitychan.fbx");

	//testModel->LoadFbxAnimation("assets/model/animation/unitychan_WAIT00.fbx");

	// プロジェクション変換行列初期化
	XMFLOAT3 eye = { 0.0f, 5.0f, -10.0f };				// 視点
	XMFLOAT3 lookat = { 0,0,0 };			// 注視点
	XMFLOAT3 up = { 0,1,0 };					// 上向きベクトル

	CCamera::GetInstance()->Init(1.0f, 10000.0f, XM_PIDIV2, SCREEN_X, SCREEN_Y, eye, lookat, up);

	// 平行光源初期化
	DX11LightInit(DirectX::XMFLOAT4(1, 1, -1, 0));		// 平行光源の方向をセット

	m_NowScene->Init();
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
	
	//シーン遷移
	SceneBase* Next_ = m_NowScene->NextScene();//シーン遷移先を設定
	if (Next_ != NULL) {
		//シーン遷移先が返ってきたら
		//次のシーンへ
		delete m_NowScene;
		m_NowScene = Next_;

		m_NowScene->Init();//初期化処理
	}

	teststage.Update();
	m_NowScene->Update();//シーン毎の更新
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

	//ワールド変換行列をセット
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, g_testmat);

	teststage.Draw();///
	m_NowScene->Render();
	//testModel->Draw();

	// レンダリング後処理
	DX11AfterRender();
}

void CGameMain::Exit() {
	DX11SetTransform::GetInstance()->Uninit();
	//testModel->UnInit();
	//delete testModel;
	delete m_NowScene;
	DX11Uninit();
}