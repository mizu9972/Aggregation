#include "Character.h"

void Player::Init() {
	Character::Init();

}

void Player::UnInit() {

}

void Player::Update() {

	Character::Move(true);//�ړ�(�L�[���삪�������������true)

	// �p�x�ݒ胊�Z�b�g
	m_Angle.x = 0.0f;
	m_Angle.y = 0.0f;
}

void Player::Draw() {

}

//�L�[���͂ɑ΂��鏈��
void Player::Up() {
	//��ړ�
	m_Angle.x = 1.0f;
	m_Angle.y = 0.0f;
}

void Player::Down() {
	//���ړ�
	m_Angle.x = -1.0f;
	m_Angle.y = 0.0f;
}

void Player::Right() {
	//�E�ړ�
	m_Angle.x = 0.0f;
	m_Angle.y = 1.0f;
}

void Player::Left() {
	//���ړ�
	m_Angle.x = 0.0f;
	m_Angle.y = -1.0f;
}