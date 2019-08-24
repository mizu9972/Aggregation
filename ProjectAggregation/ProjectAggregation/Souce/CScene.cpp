#include "CScene.h"
#include "CGameMain.h"
#include "CDirectInput.h"
//�e�V�[�������t�@�C��
//�Q�[�����C���V�[��������"GamePlayScene"�t�@�C��

//�^�C�g���V�[��--------------------------------
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

//���U���g�V�[��--------------------------------
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