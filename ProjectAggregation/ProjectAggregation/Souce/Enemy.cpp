#include "Character.h"
#include "ObjectSetting.h"
#include "WindowSetting.h"
#include "DX11Settransform.h"
#include "FileLoader.h"
#include "CGameMain.h"

void Enemy::Init() {
	Character::Init();

}

void Enemy::UnInit() {

}

void Enemy::Update() {

	RandomMove();
	Character::Move(true);//���true
}

void Enemy::RandomMove() {
	//�I�[�g�p�C���b�g���̓�������肷��

	if (m_ActionContinueTime <= 0) {

		int SetActionNum = rand() % Enemy::ACTION_NUM - 1;//�s������
		int SetActionTime = rand() % (RANDOM_MOVE_CONTINUE_TIME_MAX + RANDOM_MOVE_CONTINUE_TIME_MIN) + RANDOM_MOVE_CONTINUE_TIME_MIN;//�s���p�����Ԍ���

		//�K�p
		m_Action = SetActionNum;
		m_ActionContinueTime = SetActionTime * FPS;
	}
	m_ActionContinueTime -= 1;

	//�ړ������ݒ�
	switch (m_Action) {

	case UP:
		m_Angle.x = 1.0f;
		m_Angle.y = 0.0f;
		break;

	case DOWN:
		m_Angle.x = -1.0f;
		m_Angle.y = 0.0f;
		break;

	case RIGHT:
		m_Angle.x = 0.0f;
		m_Angle.y = 1.0f;
		break;

	case LEFT:
		m_Angle.x = 0.0f;
		m_Angle.y = -1.0f;
		break;

	case STRAIGHT:
		m_Angle.x = 0.0f;
		m_Angle.y = 0.0f;
		break;

	}
}

void Enemy::Draw() {
	//�`��

	//���[���h���W�ϊ���`�悷��
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_Matrix);
	CFileLoader::GetInstance()->Draw(CFileLoader::FileList::AirPlane);
}

void Enemy::HitFunction() {
	//��e������
	Character::HitFunction();
	
}