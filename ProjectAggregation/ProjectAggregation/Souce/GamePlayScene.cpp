#include "CDirectInput.h"
#include "CScene.h"
#include "CModel.h"
#include "CGameMain.h"

//ゲームシーン----------------------------------
GameScene::GameScene() {
	StageModel = new CModel;
}
void GameScene::Init() {
	StageModel->Init("assets/model/Ground.x.dat", "Shader/vs.fx", "Shader/MaterialColor_ps.fx");
	CGameMain::GetInstance()->AddObsever(this);
	CGameMain::GetInstance()->FeedInStart(1.0f, XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 0));
	isControlActive = false;
}

void GameScene::Update() {
	if (isControlActive == false) {
		return;
	}

	//操作受付
}

void GameScene::Render() {
	StageModel->Draw();
}

void GameScene::UnInit() {
	StageModel->Uninit();
	delete StageModel;
}

SceneBase* GameScene::NextScene() {
	return NULL;
}

void GameScene::OnNotify() {
	isControlActive = true;
	CGameMain::GetInstance()->RemoveObserver(this);
}
//--------------------------------------------