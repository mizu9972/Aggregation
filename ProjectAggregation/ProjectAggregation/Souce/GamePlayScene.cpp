
#include "CScene.h"
#include "CModel.h"
#include "CGameMain.h"
#include "Draw2D.h"
#include "WindowSetting.h"
#include "ObjectSetting.h"
#include "ScreenPosJudger.h"
#include "Command.h"

//モデルファイル
constexpr auto STAGE_MODEL_NAME = "assets/model/Ground.x.dat";
constexpr auto SKYDOME_MODEL_NAME = "assets/model/skydome.x.dat";
constexpr auto COCKPIT_MODEL_NAME = "assets/model/CockPit.x.dat";
//ゲームシーン----------------------------------
GameScene::GameScene() {
}
void GameScene::Init() {
	//初期化
	if (StageModel == nullptr) {
		StageModel = new CModel;
	}
	if (SkyDome == nullptr) {
		SkyDome = new CModel;
	}
	if (CockPit == nullptr) {
		CockPit = new CModel;
	}
	if (Site == nullptr) {
		Site = new Draw2D;
	}
	if (ActiveSite == nullptr) {
		ActiveSite = new Draw2D;
	}

	//x.datモデル読み込み
	StageModel->Init(STAGE_MODEL_NAME, "Shader/vs.fx", "Shader/MaterialColor_ps.fx");
	SkyDome->Init(SKYDOME_MODEL_NAME, "Shader/vs.fx", "Shader/psskydome.fx");
	CockPit->Init(COCKPIT_MODEL_NAME, "Shader/vs.fx", "Shader/psCockPit.fx");

	//テクスチャ読み込み
	Site->Init(SCREEN_X / 2, SCREEN_Y / 2, 0, SITE_SIZE, SITE_SIZE, XMFLOAT4(1, 1, 1, 1), "assets/textures/Site.png");
	ActiveSite->Init(SCREEN_X / 2, SCREEN_Y / 2, 0, SITE_SIZE, SITE_SIZE, XMFLOAT4(1, 1, 1, 1), "assets/textures/ActiveSite.png");
	
	//キャラクター設定
	//プレイヤー
	m_Player = new Player;

	m_Player->Init();

	//エネミー初期位置
	//エネミーの数と同じだけ用意する
	XMFLOAT3 EnemyInitPos[] = {
		{ 0.0f,0.0f,100.0f }
	};

	//エネミーの初期化
	for (int EnemyInitNum = 0; EnemyInitNum < ENEMY_START_NUM; EnemyInitNum++) {
		Character* SetEnemy = new Enemy;

		//初期化
		SetEnemy->Init();
		SetEnemy->SetTransform(EnemyInitPos[EnemyInitNum]);
		
		//リスト追加
		m_CharacterList.emplace_back(SetEnemy);
	}
	CGameMain::GetInstance()->AddObsever(this);
	CGameMain::GetInstance()->FeedInStart(1.0f, XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 0));
	isControlActive = false;
}

void GameScene::Update() {
	if (isControlActive == false) {
		return;
	}

	//操作受付
	Command* GetCommand;
	GetCommand = InputHundler::getInstance()->GetInputKey();
	if (GetCommand != nullptr) {
		GetCommand->Action(m_Player->getInstanceAtPlayerableObject());
	}

	m_Player->Update();

	//キャラクター更新
	for (int CharacterNum = 0; CharacterNum < m_CharacterList.size(); CharacterNum++) {
		m_CharacterList[CharacterNum]->Update();
	}
}

void GameScene::Render() {
	//描画
	SkyDome->Draw();
	CockPit->Draw();

	//キャラクター描画
	m_Player->Draw();
	for (int CharacterNum = 0; CharacterNum < m_CharacterList.size(); CharacterNum++) {
		m_CharacterList[CharacterNum]->Draw();
	}

	//標準サイト描画
	//サイト内にエネミーが入っているかどうか判定して描画を変える
	bool isAnyScreenIn = false;
	for (int EnemyNum = 0; EnemyNum < m_CharacterList.size(); EnemyNum++) {
		if (ScreenPosComputer::GetInstance()->JudgeSiteIn(m_CharacterList[EnemyNum]->GetPos())) {
			isAnyScreenIn = true;
		}
	}
	if (isAnyScreenIn == true) {
		ActiveSite->Draw();
	}
	else {
		Site->Draw();
	}

}

void GameScene::UnInit() {
	//終了処理
	StageModel->Uninit();
	SkyDome->Uninit();
	CockPit->Uninit();
	Site->Uninit();

	//キャラクター終了処理
	m_Player->UnInit();
	for (int CharacterNum = 0; CharacterNum < m_CharacterList.size(); CharacterNum++) {
		m_CharacterList[CharacterNum]->UnInit();
	}
	m_CharacterList.clear();
	m_CharacterList.shrink_to_fit();

	delete StageModel;
	delete SkyDome;
	delete CockPit;
	delete Site;
}

SceneBase* GameScene::NextScene() {
	return NULL;
}

void GameScene::OnNotify() {
	isControlActive = true;
	CGameMain::GetInstance()->RemoveObserver(this);
}
//--------------------------------------------