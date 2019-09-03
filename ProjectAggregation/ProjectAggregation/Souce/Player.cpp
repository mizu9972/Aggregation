#include "Character.h"
#include "CGameMain.h"
#include "CScene.h"
#include "CCamera.h"

void Player::Init() {
	Character::Init();

	LookAtPoint = CCamera::GetInstance()->GetLookat();
}

void Player::UnInit() {

}

void Player::Update() {

	//Character::Move(true);//�ړ�(�L�[���삪�������������true)
	CCamera::GetInstance()->SetLookat(LookAtPoint);
	CCamera::GetInstance()->CreateCameraMatrix();
	// �p�x�ݒ胊�Z�b�g
	m_Angle.x = 0.0f;
	m_Angle.y = 0.0f;
}

void Player::Draw() {
	//�`��
}

//�L�[���͂ɑ΂��鏈��
void Player::Up() {
	//��ړ�
	m_Angle.x = 1.0f;
	m_Angle.y = 0.0f;

	LookAtPoint.y += 0.1f;
}

void Player::Down() {
	//���ړ�
	m_Angle.x = -1.0f;
	m_Angle.y = 0.0f;

	LookAtPoint.y -= 0.1f;
}

void Player::Right() {
	//�E�ړ�
	m_Angle.x = 0.0f;
	m_Angle.y = 1.0f;

	LookAtPoint.x += 0.1f;
}

void Player::Left() {
	//���ړ�
	m_Angle.x = 0.0f;
	m_Angle.y = -1.0f;

	LookAtPoint.x -= 0.1f;
}

void Player::Act() {
	//�A�N�V����
	//�U��

	//�����蔻�肷�鏈���Ăяo��
	CGameMain::GetInstance()->getNowScene()->ObjectHitJudge();
}