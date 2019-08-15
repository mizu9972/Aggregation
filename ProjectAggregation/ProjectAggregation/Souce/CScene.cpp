#include "CScene.h"
#include "CDirectInput.h"
//タイトルシーン--------------------------------
void TitleScene::Init() {

}

void TitleScene::Update() {

}

void TitleScene::Render() {

}

SceneBase* TitleScene::NextScene(){
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN)) {
		return new GameScene;
	}
	return NULL;
}
//--------------------------------------------

//ゲームシーン----------------------------------
void GameScene::Init() {

}

void GameScene::Update() {

}

void GameScene::Render() {

}

SceneBase* GameScene::NextScene() {
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

SceneBase* ResultScene::NextScene() {
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN)) {
		return new TitleScene;
	}
	return NULL;
}
//--------------------------------------------