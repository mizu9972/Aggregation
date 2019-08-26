#include "CScene.h"
#include "CGameMain.h"
#include "CDirectInput.h"
//各シーン処理ファイル
//ゲームメインシーン処理は"GamePlayScene"ファイル

//タイトルシーン--------------------------------
void TitleScene::Init() {

}

void TitleScene::Update() {
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN)) {
		CGameMain::GetInstance()->FeedInStart(1.0f, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(1, 1, 1, 1));
		CGameMain::GetInstance()->AddObsever(this);
	}
}

void TitleScene::Render() {

}

void TitleScene::UnInit() {

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

}

void ResultScene::Update() {

}

void ResultScene::Render() {

}

void ResultScene::UnInit() {

}

SceneBase* ResultScene::NextScene() {
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN)) {
		return new TitleScene;
	}
	return NULL;
}
//--------------------------------------------