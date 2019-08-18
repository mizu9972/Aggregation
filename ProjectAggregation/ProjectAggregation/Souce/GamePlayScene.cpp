#include "CDirectInput.h"
#include "CScene.h"
#include "CModel.h"

//�Q�[���V�[��----------------------------------
GameScene::GameScene() {
	StageModel = new CModel;
}
void GameScene::Init() {
	StageModel->Init("assets/model/Ground.x.dat", "Shader/vs.fx", "Shader/MaterialColor_ps.fx");
}

void GameScene::Update() {

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
//--------------------------------------------