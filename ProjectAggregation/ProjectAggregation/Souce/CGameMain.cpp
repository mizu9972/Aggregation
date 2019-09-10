#include "CGameMain.h"
#include "DX11util.h"
#include "dx11mathutil.h"
#include "CCamera.h"
#include "CDirectInput.h"
#include "WindowSetting.h"
#include "definer.h"
#include "DX11Settransform.h"
#include "CScene.h"
#include "FileLoader.h"
#include "ObjectSetting.h"
#include <time.h>

//-----------------------------------------------
//ゲームメイン処理クラス
//-----------------------------------------------

bool CGameMain::Init(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen) {
	bool		sts;
	m_NowScene = new TitleScene;//開始シーン設定 基本はタイトルシーンから始める

	//SceneBase* temp = &SceneInstance::Game;//インスタンスを利用して上書きの例
	
	// DX11初期処理
	sts = DX11Init(hwnd, width, height, fullscreen);
	if (!sts) {
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}
	srand((unsigned)time(NULL));
	
	// DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(hinst, hwnd, width, height);
	DX11MatrixIdentity(CommonWorldMat);

	// プロジェクション変換行列初期化
	XMFLOAT3 eye    = { 0.0f, 0.0f,  0.0f };			// 視点
	XMFLOAT3 lookat = {	0.0f, 0.0f, 10.0f };			// 注視点
	XMFLOAT3 up     = { 0.0f, 1.0f,  0.0f };			// 上向きベクトル

	CCamera::GetInstance()->Init(1.0f, 10000.0f, XM_PIDIV2, SCREEN_X, SCREEN_Y, eye, lookat, up);

	// 平行光源初期化
	DX11LightInit(DirectX::XMFLOAT4(1, 1, -1, 0));		// 平行光源の方向をセット

	// ファイル管理クラス初期化
	CFileLoader::GetInstance()->Init();

	
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

	m_NowScene->Update();//シーン毎の更新

	for (unsigned int EffectNo = 0; EffectNo < m_EffectList.size(); EffectNo++) {
		m_EffectList[EffectNo]->Update();
	}
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

	m_NowScene->Render();

	for (unsigned int EffectNo = 0; EffectNo < m_EffectList.size();EffectNo++) {
		m_EffectList[EffectNo]->Draw();
	}

	// レンダリング後処理
	DX11AfterRender();
}

void CGameMain::Exit() {
	DX11SetTransform::GetInstance()->Uninit();
	delete m_NowScene;
	DX11Uninit();
}

void CGameMain::FeedInStart(float MaxTime, XMFLOAT4 StartColor, XMFLOAT4 EndColor) {
	//フェードイン開始処理
	CEffectiveObject *Feedin_;
	Feedin_ = new CFeedIn;
	Feedin_->Init(MaxTime, StartColor, EndColor);//初期化(時間,開始色,終了色)
	Feedin_->AddObsever(CGameMain::GetInstance());//オブザーバリストに追加
	m_EffectList.emplace_back(Feedin_);//エフェクトリストへ追加
}

void CGameMain::OnNotify() {
	//エフェクト終了通知受け取り

	//エフェクトリストから削除
	for (unsigned int ListNum = 0; ListNum < m_EffectList.size(); ListNum++) {
		if (m_EffectList[ListNum]->GetActive() == false) {
			m_EffectList[ListNum]->RemoveObserver(CGameMain::GetInstance());//オブザーバ削除
			m_EffectList.erase(m_EffectList.begin() + ListNum);
			m_EffectList.shrink_to_fit();
		}
	}

	Notify();//通知
}
