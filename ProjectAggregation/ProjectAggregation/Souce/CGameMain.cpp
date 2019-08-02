#include "CGameMain.h"
#include "DX11util.h"
#include "CCamera.h"
#include "CDirectInput.h"
#include "WindowSetting.h"
#include "definer.h"
#include "DX11Settransform.h"


//-----------------------------------------------
//�Q�[�����C�������N���X
//-----------------------------------------------

bool CGameMain::Init(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen) {
	bool		sts;

	// DX11��������
	sts = DX11Init(hwnd, width, height, fullscreen);
	if (!sts) {
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}

	// DIRECTINPUT������
	CDirectInput::GetInstance().Init(hinst, hwnd, width, height);

	testModel->Load("assets/model/EditStage.fbx");

	// �v���W�F�N�V�����ϊ��s�񏉊���
	XMFLOAT3 eye = { 0,5,-5 };				// ���_
	XMFLOAT3 lookat = { 0,0,0 };			// �����_
	XMFLOAT3 up = { 0,1,0 };					// ������x�N�g��

	CCamera::GetInstance()->Init(1.0f, 10000.0f, XM_PIDIV2, SCREEN_X, SCREEN_Y, eye, lookat, up);

	// ���s����������
	DX11LightInit(DirectX::XMFLOAT4(1, 1, -1, 0));		// ���s�����̕������Z�b�g

	//testModel->LoadFbxAnimation("assets/model/animation/dousakakuninn_ver01.fbx");
	
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

	testModel->Draw();

	// �����_�����O�㏈��
	DX11AfterRender();
}

void CGameMain::Exit() {
	DX11SetTransform::GetInstance()->Uninit();	DX11Uninit();
	DX11Uninit();
}