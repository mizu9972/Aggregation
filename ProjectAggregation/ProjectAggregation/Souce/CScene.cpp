#include "CScene.h"
#include "CGameMain.h"
#include "CDirectInput.h"
#include "Draw2D.h"
#include "WindowSetting.h"

//�e�V�[�������t�@�C��
//�Q�[�����C���V�[��������"GamePlayScene"�t�@�C��

//�e�N�X�`���t�@�C��
constexpr auto TITLE_TEXTURE = "assets/textures/Title.png";
constexpr auto RESULT_TEXTURE = "assets/textures/Result.png";

//�^�C�g���V�[��--------------------------------
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
	//�ʒm�󂯎��
	CGameMain::GetInstance()->RemoveObserver(this);
	isTitleEnd = true;
}
//--------------------------------------------

//���U���g�V�[��--------------------------------
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