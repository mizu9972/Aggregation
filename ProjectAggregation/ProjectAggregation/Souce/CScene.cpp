#include "CScene.h"
#include "CGameMain.h"
#include "CDirectInput.h"
#include "Draw2D.h"
#include "WindowSetting.h"

//各シーン処理ファイル
//ゲームメインシーン処理は"GamePlayScene"ファイル

//テクスチャファイル
constexpr auto TITLE_TEXTURE = "assets/textures/Title.png";
constexpr auto RESULT_TEXTURE = "assets/textures/Result.png";

//タイトルシーン--------------------------------
void TitleScene::Init() {
	if (m_TitleTex == nullptr) {
		m_TitleTex = new Draw2D;
	}
	m_TitleTex->Init(SCREEN_X / 2, SCREEN_Y / 2, 0, SCREEN_X, SCREEN_Y, XMFLOAT4(1, 1, 1, 1), TITLE_TEXTURE);
}

void TitleScene::Update() {
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN)) {
		CGameMain::GetInstance()->FeedInStart(1.0f, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(1, 1, 1, 1));
		CGameMain::GetInstance()->AddObsever(this);
	}
}

void TitleScene::Render() {
	m_TitleTex->Draw();
}

void TitleScene::UnInit() {
	m_TitleTex->Uninit();
}

SceneBase* TitleScene::NextScene(){
	if (isTitleEnd == true) {

		return new GameScene;
	}
	return NULL;
}

void TitleScene::OnNotify() {
	//通知受け取り
	CGameMain::GetInstance()->RemoveObserver(this);
	isTitleEnd = true;
}
//--------------------------------------------

//リザルトシーン--------------------------------
void ResultScene::Init() {
	if (m_ResultTex == nullptr) {
		m_ResultTex = new Draw2D;
	}
	m_ResultTex->Init(SCREEN_X / 2, SCREEN_Y / 2, 0, SCREEN_X, SCREEN_Y, XMFLOAT4(1, 1, 1, 1), RESULT_TEXTURE);
}

void ResultScene::Update() {

}

void ResultScene::Render() {
	m_ResultTex->Draw();
}

void ResultScene::UnInit() {
	m_ResultTex->Uninit();
}

SceneBase* ResultScene::NextScene() {
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
		return new TitleScene;
	}
	return NULL;
}
//--------------------------------------------