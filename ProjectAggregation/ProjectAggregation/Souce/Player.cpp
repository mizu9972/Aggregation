#include "Character.h"
#include "CGameMain.h"
#include "CScene.h"
#include "CCamera.h"
#include "DX11Settransform.h"
#include "FileLoader.h"
#include "ObjectSetting.h"

void Player::Init() {
	Character::Init();

}

void Player::UnInit() {

}

void Player::Update() {

	Character::Move(true);//�ړ�(�L�[���삪�������������true)

	//�J��������
	CameraMove();

	// �p�x�ݒ胊�Z�b�g
	m_Angle.x = 0.0f;
	m_Angle.y = 0.0f;
	m_Angle.z = 0.0f;
}

void Player::CameraMove() {
	//�J��������
	XMFLOAT3 Eye_;
	XMFLOAT3 Up_;
	XMFLOAT3 LookAt_;
	
	//���_�����O����
	LookAt_.x = m_Matrix._41 + m_Matrix._31 * 5.0f;
	LookAt_.y = m_Matrix._42 + m_Matrix._32 * 5.0f;
	LookAt_.z = m_Matrix._43 + m_Matrix._33 * 5.0f;

	//�ʒu�E�X���̓v���C���[�ɂ��킹��
	//�J�����͂��炳�����f�������炷
	Eye_.x = m_Matrix._41;
	Eye_.y = m_Matrix._42;
	Eye_.z = m_Matrix._43;
	Up_.x  = m_Matrix._21;
	Up_.y  = m_Matrix._22;
	Up_.z  = m_Matrix._23;

	CCamera::GetInstance()->SetCamera(Eye_, LookAt_, Up_);
	XMFLOAT4X4  mat = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mat);
}

void Player::Draw() {
	//�`��
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_Matrix);
	CFileLoader::GetInstance()->Draw(CFileLoader::FileList::CockPit);
}

//�L�[���͂ɑ΂��鏈��
void Player::Up() {
	//��ړ�
	m_Angle.x += 1.0f;
	m_Angle.y += 0.0f;

}

void Player::Down() {
	//���ړ�
	m_Angle.x += -1.0f;
	m_Angle.y += 0.0f;

}

void Player::Right() {
	//�E�ړ�
	m_Angle.x += 0.0f;
	m_Angle.y += 1.0f;

}

void Player::Left() {
	//���ړ�
	m_Angle.x += 0.0f;
	m_Angle.y += -1.0f;

}

void Player::R_Turn() {
	//�E��]
	m_Angle.z -= 1.0f;
}

void Player::L_Turn() {
	//����]
	m_Angle.z += 1.0f;
}

void Player::Act() {
	//�A�N�V����
	//�U��

	//�����蔻�肷�鏈���Ăяo��
	CGameMain::GetInstance()->getNowScene()->ObjectHitJudge();
}