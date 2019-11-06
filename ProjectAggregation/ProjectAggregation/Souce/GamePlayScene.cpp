
#include "CCamera.h"
#include "CScene.h"
#include "CModel.h"
#include "CGameMain.h"
#include "Draw2D.h"
#include "WindowSetting.h"
#include "ObjectSetting.h"
#include "ScreenPosJudger.h"
#include "Command.h"
#include "FileLoader.h"

//ゲームプレイシーンの処理

//モデルファイル
constexpr auto STAGE_MODEL_NAME = "assets/model/Ground.x.dat";
constexpr auto SKYDOME_MODEL_NAME = "assets/model/skydome.x.dat";
//ゲームシーン----------------------------------
GameScene::GameScene() {
}

void GameScene::Init() {
	//初期化
	if (StageModel == nullptr) {
		StageModel = new CModel;
	}
	if (Site       == nullptr) {
		Site       = new Draw2D;
	}
	if (ActiveSite == nullptr) {
		ActiveSite = new Draw2D;
	}

	//x.datモデル読み込み
	StageModel->Init(STAGE_MODEL_NAME,   "Shader/vs.fx", "Shader/MaterialColor_ps.fx");

	//テクスチャ読み込み
	Site->Init(		 SCREEN_X / 2, SCREEN_Y / 2, 0, SITE_SIZE, SITE_SIZE, XMFLOAT4(1, 1, 1, 1), "assets/textures/Site.png");
	ActiveSite->Init(SCREEN_X / 2, SCREEN_Y / 2, 0, SITE_SIZE, SITE_SIZE, XMFLOAT4(1, 1, 1, 1), "assets/textures/ActiveSite.png");
	
	//キャラクター設定
	//プレイヤー
	m_Player = new Player;

	m_Player->Init();

	//エネミー初期位置
	//エネミーの数と同じだけ用意する
	XMFLOAT3 EnemyInitPos[] = {
		//     x       y       z     No
		{   0.0f,   0.0f,  60.0f }, //1
		{  10.0f,   0.0f,  70.0f }, //2
		{   0.0f,  20.0f,  70.0f }, //3
		{  20.0f,  10.0f,  80.0f }, //4
		{ -20.0f, -10.0f,  90.0f }  //5

	};

	//エネミーの初期化
	for (int EnemyInitNum = 0; EnemyInitNum < ENEMY_START_NUM; EnemyInitNum++) {
		Character* SetEnemy = new Enemy;

		//初期化
		SetEnemy->Init();
		SetEnemy->SetTransform(EnemyInitPos[EnemyInitNum]);
		SetEnemy->AddObsever(this);
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

	InputState::GetInstance()->Update();

	m_Player->Update();

	//キャラクター更新
	for (unsigned int CharacterNum = 0; CharacterNum < m_CharacterList.size(); CharacterNum++) {
		m_CharacterList[CharacterNum]->Update();
	}

	//パーティクル更新
	for (unsigned int ParticleNum = 0; ParticleNum < m_ParticleList.size(); ParticleNum++) {
		if (m_ParticleList[ParticleNum]->GetSystemActivate() == false) {
			continue;
		}
		m_ParticleList[ParticleNum]->Update();
	}

	//カメラ更新
	XMFLOAT4X4 CameraMatrix = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, CameraMatrix);
}

void GameScene::Render() {
	//描画

	//スカイドーム->キャラクター等のオブジェクト->UI等の順に描画
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, CommonWorldMat);
	CFileLoader::GetInstance()->Draw(CFileLoader::FileList::SkyDome);

	//キャラクター描画
	for (unsigned int CharacterNum = 0; CharacterNum < m_CharacterList.size(); CharacterNum++) {
		m_CharacterList[CharacterNum]->Draw();
	}
	m_Player->Draw();

	//標準サイト描画
	//サイト内にエネミーが入っているかどうか判定して描画を変える
	bool isAnyScreenIn = false;
	for (unsigned int EnemyNum = 0; EnemyNum < m_CharacterList.size(); EnemyNum++) {
		if (ScreenPosComputer::GetInstance()->JudgeSiteIn(m_CharacterList[EnemyNum]->GetPos())) {
			isAnyScreenIn = true;
		}
	}

	//パーティクル描画
	for (unsigned int ParticleNum = 0; ParticleNum < m_ParticleList.size(); ParticleNum++) {
		m_ParticleList[ParticleNum]->Draw();
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
	Site->Uninit();

	//キャラクター終了処理
	m_Player->UnInit();
	for (unsigned int CharacterNum = 0; CharacterNum < m_CharacterList.size(); CharacterNum++) {
		m_CharacterList[CharacterNum]->UnInit();
	}
	m_CharacterList.clear();
	m_CharacterList.shrink_to_fit();

	//パーティクル終了処理
	for (unsigned int ParticleNum = 0; ParticleNum < m_ParticleList.size(); ParticleNum++) {
		m_ParticleList[ParticleNum]->UnInit();
	}
	m_ParticleList.clear();
	m_ParticleList.shrink_to_fit();

	delete StageModel;
	delete Site;
}

void GameScene::ObjectHitJudge() {
	//当たり判定
	//エネミー
	for (unsigned int EnemyNum = 0; EnemyNum < m_CharacterList.size(); EnemyNum++) {
		if (ScreenPosComputer::GetInstance()->JudgeSiteIn(m_CharacterList[EnemyNum]->GetPos())) {
			//#TODO
			//標準サイト内のすべてのエネミーに当たっている
			//Z軸ベクトルの数値で判定するなどして一番手前のエネミーのみ処理するようにしたい

			//#TODO
			//新しくパーティクルを生成する際に、ファイル読み込みから初期化するのではなくて
			//複製などもっと簡易的な処理にできないか

			//パーティクル生成
			ParticleSystem* SetParticle = new ParticleSystem;
			SetParticle->FInState("ParticleData/ExplosionData.txt", "assets/textures/NomalParticle.png");
			XMFLOAT3 SetPos = m_CharacterList[EnemyNum]->GetPos();
			SetParticle->SetPos(SetPos.x, SetPos.y, SetPos.z);
			SetParticle->Start();

			m_ParticleList.emplace_back(SetParticle);//リストに追加

			m_CharacterList[EnemyNum]->HitFunction();


		}
	}

	
	//障害物への攻撃当たり判定
}

SceneBase* GameScene::NextScene() {
	if (m_CharacterList.size() <= 0) {
		return new ResultScene;
	}
	return NULL;
}

void GameScene::OnNotify() {
	isControlActive = true;
	CGameMain::GetInstance()->RemoveObserver(this);
}

void GameScene::OnNotify(Subject* subject_) {
	//エネミー死亡通知受け取り
	for (unsigned int EnemyNum = 0; EnemyNum < m_CharacterList.size(); EnemyNum++) {
		if (m_CharacterList[EnemyNum] == subject_) {
			m_CharacterList.erase(m_CharacterList.begin() + EnemyNum);
		}
	}
	m_CharacterList.shrink_to_fit();
}
//--------------------------------------------