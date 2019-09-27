#include "Character.h"
#include "CGameMain.h"
#include "CScene.h"
#include "CCamera.h"
#include "DX11Settransform.h"
#include "dx11mathutil.h"
#include "FileLoader.h"
#include "ObjectSetting.h"

void Player::Init() {
	Character::Init();

	m_CameraAngle.x = 90.0f;
	m_CameraAngle.y = 90.0f;
	m_CameraAngle.z = 0.0f;
}

void Player::UnInit() {

}

void Player::Update() {

	//�p�x�������֐�
	auto AngleReset = [](XMFLOAT3& Angle_) { 
		Angle_.x = 0.0f;
		Angle_.y = 0.0f;
		Angle_.z = 0.0f;
	};


	Character::Move(true);//�ړ�(�L�[���삪�������������true)

	//�J��������
	CameraMove();

	// �p�x�ݒ胊�Z�b�g
	AngleReset(m_Angle);
}

void Player::CameraMove() {
	//�J�����̑���
	
	//�p�x�����W�A���ɕϊ��֐�
	auto ToRadian = [](float Angle) { return (Angle * XM_PI) / 180.0; };
	
	XMFLOAT3 Eye_;
	XMFLOAT3 Up_;
	XMFLOAT3 LookAt_;

	
	
	//�ʒu�E�X���̓v���C���[�ɂ��킹��
	//�J�����͂��炳�����f�������炷
	Eye_.x = m_Matrix._41;
	Eye_.y = m_Matrix._42;
	Eye_.z = m_Matrix._43;
	Up_.x  = m_Matrix._21;
	Up_.y  = m_Matrix._22;
	Up_.z  = m_Matrix._23;

	//��]���W�擾
	float HorizonAgnle = CCamera::GetInstance()->GetHorizonAngle(&Eye_, &LookAt_);
	float VerticalAngle = CCamera::GetInstance()->GetVerticalAngle(&Eye_, &LookAt_);

	//���_�ړ��ʉ��Z
	HorizonAgnle += m_CameraAngle.x;
	VerticalAngle += m_CameraAngle.y;
	XMFLOAT3 SetAngle = { HorizonAgnle,VerticalAngle,0.0f };

	float r = 5.0f + sinf(ToRadian(VerticalAngle));
	//���_�����O����
	LookAt_.x = m_Matrix._41 + m_Matrix._31 * 5.0f;
	LookAt_.y = m_Matrix._42 + m_Matrix._32 * 5.0f;
	LookAt_.z = m_Matrix._43 + m_Matrix._33 * 5.0f;

	//XMFLOAT4X4 LookMat;
	//DX11MakeWorldMatrix(LookMat, SetAngle, LookAt_);

	//LookAt_.x = LookMat._41;
	//LookAt_.y = LookMat._42;
	//LookAt_.z = LookMat._43;

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

//�J��������
void Player::SubUp() {
if (1/*m_CameraAngle.y >= LOOKAT_LIMIT_ANGLE*/) {
		m_CameraAngle.y -= 10.0f;
	}
}

void Player::SubDown() {
	if (1/*m_CameraAngle.y <= 180 - LOOKAT_LIMIT_ANGLE*/) {
		m_CameraAngle.y += 10.0f;
	}	
}

void Player::SubRight() {
	if (1/*m_CameraAngle.x >= LOOKAT_LIMIT_ANGLE*/) {
		m_CameraAngle.x -= 10.0f;
	}
}

void Player::SubLeft() {
	if (1/*m_CameraAngle.x <= 180 - LOOKAT_LIMIT_ANGLE*/) {
		m_CameraAngle.x += 10.0f;
	}
}