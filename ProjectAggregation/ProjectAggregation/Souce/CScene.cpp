#include "CScene.h"
#include "CGameMain.h"
#include "CDirectInput.h"
//�e�V�[�������t�@�C��
//�Q�[�����C���V�[��������"GamePlayScene"�t�@�C��

//�^�C�g���V�[��--------------------------------
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
	//�ʒm�󂯎��
	CGameMain::GetInstance()->RemoveObserver(this);
	isTitleEnd = true;
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