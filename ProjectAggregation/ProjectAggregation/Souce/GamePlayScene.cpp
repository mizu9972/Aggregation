#include "CDirectInput.h"
#include "CScene.h"
#include "CModel.h"
#include "CGameMain.h"

//���f���t�@�C��
constexpr auto STAGE_MODEL_NAME = "assets/model/Ground.x.dat";
constexpr auto SKYDOME_MODEL_NAME = "assets/model/skydome.x.dat";
constexpr auto COCKPIT_MODEL_NAME = "assets/model/CockPit.x.dat";
//�Q�[���V�[��----------------------------------
GameScene::GameScene() {
}
void GameScene::Init() {
	//������
	if (StageModel == nullptr) {
		StageModel = new CModel;
	}
	if (SkyDome == nullptr) {
		SkyDome = new CModel;
	}
	if (CockPit == nullptr) {
		CockPit = new CModel;
	}

	//x.dat���f���ǂݍ���
	StageModel->Init(STAGE_MODEL_NAME, "Shader/vs.fx", "Shader/MaterialColor_ps.fx");
	SkyDome->Init(SKYDOME_MODEL_NAME, "Shader/vs.fx", "Shader/psskydome.fx");
	CockPit->Init(COCKPIT_MODEL_NAME, "Shader/vs.fx", "Shader/psCockPit.fx");
	
	CGameMain::GetInstance()->AddObsever(this);
	CGameMain::GetInstance()->FeedInStart(1.0f, XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 0));
	isControlActive = false;
}

void GameScene::Update() {
	if (isControlActive == false) {
		return;
	}

	//�����t

	
}

void GameScene::Render() {
	//�`��
	SkyDome->Draw();
	StageModel->Draw();
	CockPit->Draw();
}

void GameScene::UnInit() {
	//�I������
	StageModel->Uninit();
	SkyDome->Uninit();
	CockPit->Uninit();
	delete StageModel;
	delete SkyDome;
	delete CockPit;
}

SceneBase* GameScene::NextScene() {
	return NULL;
}

void GameScene::OnNotify() {
	isControlActive = true;
	CGameMain::GetInstance()->RemoveObserver(this);
}
//--------------------------------------------