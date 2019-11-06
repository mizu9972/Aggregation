
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

//�Q�[���v���C�V�[���̏���

//���f���t�@�C��
constexpr auto STAGE_MODEL_NAME = "assets/model/Ground.x.dat";
constexpr auto SKYDOME_MODEL_NAME = "assets/model/skydome.x.dat";
//�Q�[���V�[��----------------------------------
GameScene::GameScene() {
}

void GameScene::Init() {
	//������
	if (StageModel == nullptr) {
		StageModel = new CModel;
	}
	if (Site       == nullptr) {
		Site       = new Draw2D;
	}
	if (ActiveSite == nullptr) {
		ActiveSite = new Draw2D;
	}

	//x.dat���f���ǂݍ���
	StageModel->Init(STAGE_MODEL_NAME,   "Shader/vs.fx", "Shader/MaterialColor_ps.fx");

	//�e�N�X�`���ǂݍ���
	Site->Init(		 SCREEN_X / 2, SCREEN_Y / 2, 0, SITE_SIZE, SITE_SIZE, XMFLOAT4(1, 1, 1, 1), "assets/textures/Site.png");
	ActiveSite->Init(SCREEN_X / 2, SCREEN_Y / 2, 0, SITE_SIZE, SITE_SIZE, XMFLOAT4(1, 1, 1, 1), "assets/textures/ActiveSite.png");
	
	//�L�����N�^�[�ݒ�
	//�v���C���[
	m_Player = new Player;

	m_Player->Init();

	//�G�l�~�[�����ʒu
	//�G�l�~�[�̐��Ɠ��������p�ӂ���
	XMFLOAT3 EnemyInitPos[] = {
		//     x       y       z     No
		{   0.0f,   0.0f,  60.0f }, //1
		{  10.0f,   0.0f,  70.0f }, //2
		{   0.0f,  20.0f,  70.0f }, //3
		{  20.0f,  10.0f,  80.0f }, //4
		{ -20.0f, -10.0f,  90.0f }  //5

	};

	//�G�l�~�[�̏�����
	for (int EnemyInitNum = 0; EnemyInitNum < ENEMY_START_NUM; EnemyInitNum++) {
		Character* SetEnemy = new Enemy;

		//������
		SetEnemy->Init();
		SetEnemy->SetTransform(EnemyInitPos[EnemyInitNum]);
		SetEnemy->AddObsever(this);
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

	InputState::GetInstance()->Update();

	m_Player->Update();

	//�L�����N�^�[�X�V
	for (unsigned int CharacterNum = 0; CharacterNum < m_CharacterList.size(); CharacterNum++) {
		m_CharacterList[CharacterNum]->Update();
	}

	//�p�[�e�B�N���X�V
	for (unsigned int ParticleNum = 0; ParticleNum < m_ParticleList.size(); ParticleNum++) {
		if (m_ParticleList[ParticleNum]->GetSystemActivate() == false) {
			continue;
		}
		m_ParticleList[ParticleNum]->Update();
	}

	//�J�����X�V
	XMFLOAT4X4 CameraMatrix = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, CameraMatrix);
}

void GameScene::Render() {
	//�`��

	//�X�J�C�h�[��->�L�����N�^�[���̃I�u�W�F�N�g->UI���̏��ɕ`��
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, CommonWorldMat);
	CFileLoader::GetInstance()->Draw(CFileLoader::FileList::SkyDome);

	//�L�����N�^�[�`��
	for (unsigned int CharacterNum = 0; CharacterNum < m_CharacterList.size(); CharacterNum++) {
		m_CharacterList[CharacterNum]->Draw();
	}
	m_Player->Draw();

	//�W���T�C�g�`��
	//�T�C�g���ɃG�l�~�[�������Ă��邩�ǂ������肵�ĕ`���ς���
	bool isAnyScreenIn = false;
	for (unsigned int EnemyNum = 0; EnemyNum < m_CharacterList.size(); EnemyNum++) {
		if (ScreenPosComputer::GetInstance()->JudgeSiteIn(m_CharacterList[EnemyNum]->GetPos())) {
			isAnyScreenIn = true;
		}
	}

	//�p�[�e�B�N���`��
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
	//�I������
	StageModel->Uninit();
	Site->Uninit();

	//�L�����N�^�[�I������
	m_Player->UnInit();
	for (unsigned int CharacterNum = 0; CharacterNum < m_CharacterList.size(); CharacterNum++) {
		m_CharacterList[CharacterNum]->UnInit();
	}
	m_CharacterList.clear();
	m_CharacterList.shrink_to_fit();

	//�p�[�e�B�N���I������
	for (unsigned int ParticleNum = 0; ParticleNum < m_ParticleList.size(); ParticleNum++) {
		m_ParticleList[ParticleNum]->UnInit();
	}
	m_ParticleList.clear();
	m_ParticleList.shrink_to_fit();

	delete StageModel;
	delete Site;
}

void GameScene::ObjectHitJudge() {
	//�����蔻��
	//�G�l�~�[
	for (unsigned int EnemyNum = 0; EnemyNum < m_CharacterList.size(); EnemyNum++) {
		if (ScreenPosComputer::GetInstance()->JudgeSiteIn(m_CharacterList[EnemyNum]->GetPos())) {
			//#TODO
			//�W���T�C�g���̂��ׂẴG�l�~�[�ɓ������Ă���
			//Z���x�N�g���̐��l�Ŕ��肷��Ȃǂ��Ĉ�Ԏ�O�̃G�l�~�[�̂ݏ�������悤�ɂ�����

			//#TODO
			//�V�����p�[�e�B�N���𐶐�����ۂɁA�t�@�C���ǂݍ��݂��珉��������̂ł͂Ȃ���
			//�����Ȃǂ����ƊȈՓI�ȏ����ɂł��Ȃ���

			//�p�[�e�B�N������
			ParticleSystem* SetParticle = new ParticleSystem;
			SetParticle->FInState("ParticleData/ExplosionData.txt", "assets/textures/NomalParticle.png");
			XMFLOAT3 SetPos = m_CharacterList[EnemyNum]->GetPos();
			SetParticle->SetPos(SetPos.x, SetPos.y, SetPos.z);
			SetParticle->Start();

			m_ParticleList.emplace_back(SetParticle);//���X�g�ɒǉ�

			m_CharacterList[EnemyNum]->HitFunction();


		}
	}

	
	//��Q���ւ̍U�������蔻��
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
	//�G�l�~�[���S�ʒm�󂯎��
	for (unsigned int EnemyNum = 0; EnemyNum < m_CharacterList.size(); EnemyNum++) {
		if (m_CharacterList[EnemyNum] == subject_) {
			m_CharacterList.erase(m_CharacterList.begin() + EnemyNum);
		}
	}
	m_CharacterList.shrink_to_fit();
}
//--------------------------------------------