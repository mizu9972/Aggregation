
#include "CScene.h"
#include "CModel.h"
#include "CGameMain.h"
#include "Draw2D.h"
#include "WindowSetting.h"
#include "ObjectSetting.h"
#include "ScreenPosJudger.h"
#include "Command.h"

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
	if (Site == nullptr) {
		Site = new Draw2D;
	}
	if (ActiveSite == nullptr) {
		ActiveSite = new Draw2D;
	}

	//x.dat���f���ǂݍ���
	StageModel->Init(STAGE_MODEL_NAME, "Shader/vs.fx", "Shader/MaterialColor_ps.fx");
	SkyDome->Init(SKYDOME_MODEL_NAME, "Shader/vs.fx", "Shader/psskydome.fx");
	CockPit->Init(COCKPIT_MODEL_NAME, "Shader/vs.fx", "Shader/psCockPit.fx");

	//�e�N�X�`���ǂݍ���
	Site->Init(SCREEN_X / 2, SCREEN_Y / 2, 0, SITE_SIZE, SITE_SIZE, XMFLOAT4(1, 1, 1, 1), "assets/textures/Site.png");
	ActiveSite->Init(SCREEN_X / 2, SCREEN_Y / 2, 0, SITE_SIZE, SITE_SIZE, XMFLOAT4(1, 1, 1, 1), "assets/textures/ActiveSite.png");
	
	//�L�����N�^�[�ݒ�
	//�v���C���[
	m_Player = new Player;

	m_Player->Init();

	//�G�l�~�[�����ʒu
	//�G�l�~�[�̐��Ɠ��������p�ӂ���
	XMFLOAT3 EnemyInitPos[] = {
		{ 0.0f,0.0f,100.0f }
	};

	//�G�l�~�[�̏�����
	for (int EnemyInitNum = 0; EnemyInitNum < ENEMY_START_NUM; EnemyInitNum++) {
		Character* SetEnemy = new Enemy;

		//������
		SetEnemy->Init();
		SetEnemy->SetTransform(EnemyInitPos[EnemyInitNum]);
		
		//���X�g�ǉ�
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

	//�����t
	Command* GetCommand;
	GetCommand = InputHundler::getInstance()->GetInputKey();
	if (GetCommand != nullptr) {
		GetCommand->Action(m_Player->getInstanceAtPlayerableObject());
	}

	m_Player->Update();

	//�L�����N�^�[�X�V
	for (int CharacterNum = 0; CharacterNum < m_CharacterList.size(); CharacterNum++) {
		m_CharacterList[CharacterNum]->Update();
	}
}

void GameScene::Render() {
	//�`��
	SkyDome->Draw();
	CockPit->Draw();

	//�L�����N�^�[�`��
	m_Player->Draw();
	for (int CharacterNum = 0; CharacterNum < m_CharacterList.size(); CharacterNum++) {
		m_CharacterList[CharacterNum]->Draw();
	}

	//�W���T�C�g�`��
	//�T�C�g���ɃG�l�~�[�������Ă��邩�ǂ������肵�ĕ`���ς���
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
	//�I������
	StageModel->Uninit();
	SkyDome->Uninit();
	CockPit->Uninit();
	Site->Uninit();

	//�L�����N�^�[�I������
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