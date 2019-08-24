#include "CScene.h"
#include "CGameMain.h"
#include "CDirectInput.h"
//各シーン処理ファイル
//ゲームメインシーン処理は"GamePlayScene"ファイル

//タイトルシーン--------------------------------
void TitleScene::Init() {

}

void TitleScene::Update() {

}

void TitleScene::Render() {

}

void TitleScene::UnInit() {

}

SceneBase* TitleScene::NextScene(){
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN)) {
		CGameMain::GetInstance()->FeedInStart();
		return new GameScene;
	}
	return NULL;
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