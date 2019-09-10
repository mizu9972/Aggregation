#include "CGameMain.h"
#include "DX11util.h"
#include "dx11mathutil.h"
#include "CCamera.h"
#include "CDirectInput.h"
#include "WindowSetting.h"
#include "definer.h"
#include "DX11Settransform.h"
#include "CScene.h"
#include "FileLoader.h"
#include "ObjectSetting.h"
#include <time.h>

//-----------------------------------------------
//�Q�[�����C�������N���X
//-----------------------------------------------

bool CGameMain::Init(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen) {
	bool		sts;
	m_NowScene = new TitleScene;//�J�n�V�[���ݒ� ��{�̓^�C�g���V�[������n�߂�

	//SceneBase* temp = &SceneInstance::Game;//�C���X�^���X�𗘗p���ď㏑���̗�
	
	// DX11��������
	sts = DX11Init(hwnd, width, height, fullscreen);
	if (!sts) {
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}
	srand((unsigned)time(NULL));
	
	// DIRECTINPUT������
	CDirectInput::GetInstance().Init(hinst, hwnd, width, height);
	DX11MatrixIdentity(CommonWorldMat);

	// �v���W�F�N�V�����ϊ��s�񏉊���
	XMFLOAT3 eye    = { 0.0f, 0.0f,  0.0f };			// ���_
	XMFLOAT3 lookat = {	0.0f, 0.0f, 10.0f };			// �����_
	XMFLOAT3 up     = { 0.0f, 1.0f,  0.0f };			// ������x�N�g��

	CCamera::GetInstance()->Init(1.0f, 10000.0f, XM_PIDIV2, SCREEN_X, SCREEN_Y, eye, lookat, up);

	// ���s����������
	DX11LightInit(DirectX::XMFLOAT4(1, 1, -1, 0));		// ���s�����̕������Z�b�g

	// �t�@�C���Ǘ��N���X������
	CFileLoader::GetInstance()->Init();

	
	m_NowScene->Init();
	return true;
}

void CGameMain::Main() {
	Input();

	Update();

	Render();
}

void CGameMain::Input() {
	CDirectInput::GetInstance().GetKeyBuffer();//�L�[���͎擾
}

void CGameMain::Update() {
	
	//�V�[���J��
	SceneBase* Next_ = m_NowScene->NextScene();//�V�[���J�ڐ��ݒ�
	if (Next_ != NULL) {
		//�V�[���J�ڐ悪�Ԃ��Ă�����
		//���̃V�[����
		delete m_NowScene;
		m_NowScene = Next_;

		m_NowScene->Init();//����������
	}

	m_NowScene->Update();//�V�[�����̍X�V

	for (unsigned int EffectNo = 0; EffectNo < m_EffectList.size(); EffectNo++) {
		m_EffectList[EffectNo]->Update();
	}
}

void CGameMain::Render() {
	// �^�[�Q�b�g�o�b�t�@�N���A	
	float ClearColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	
	// �����_�����O�O����
	DX11BeforeRender(ClearColor);

	XMFLOAT4X4 mat;

	// �r���[�ϊ��s��Z�b�g
	mat = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mat);

	// �v���W�F�N�V�����ϊ��s��Z�b�g
	mat = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mat);

	m_NowScene->Render();

	for (unsigned int EffectNo = 0; EffectNo < m_EffectList.size();EffectNo++) {
		m_EffectList[EffectNo]->Draw();
	}

	// �����_�����O�㏈��
	DX11AfterRender();
}

void CGameMain::Exit() {
	DX11SetTransform::GetInstance()->Uninit();
	delete m_NowScene;
	DX11Uninit();
}

void CGameMain::FeedInStart(float MaxTime, XMFLOAT4 StartColor, XMFLOAT4 EndColor) {
	//�t�F�[�h�C���J�n����
	CEffectiveObject *Feedin_;
	Feedin_ = new CFeedIn;
	Feedin_->Init(MaxTime, StartColor, EndColor);//������(����,�J�n�F,�I���F)
	Feedin_->AddObsever(CGameMain::GetInstance());//�I�u�U�[�o���X�g�ɒǉ�
	m_EffectList.emplace_back(Feedin_);//�G�t�F�N�g���X�g�֒ǉ�
}

void CGameMain::OnNotify() {
	//�G�t�F�N�g�I���ʒm�󂯎��

	//�G�t�F�N�g���X�g����폜
	for (unsigned int ListNum = 0; ListNum < m_EffectList.size(); ListNum++) {
		if (m_EffectList[ListNum]->GetActive() == false) {
			m_EffectList[ListNum]->RemoveObserver(CGameMain::GetInstance());//�I�u�U�[�o�폜
			m_EffectList.erase(m_EffectList.begin() + ListNum);
			m_EffectList.shrink_to_fit();
		}
	}

	Notify();//�ʒm
}
