#include "CScene.h"
#include "CDirectInput.h"
//�^�C�g���V�[��--------------------------------
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

//�Q�[���V�[��----------------------------------
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

//���U���g�V�[��--------------------------------
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