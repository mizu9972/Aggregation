#include "ScreenPosJudger.h"
#include "CCamera.h"
#include "ObjectSetting.h"
#include "DX11Settransform.h"

ScreenPosComputer::ScreenPosComputer() {
	
}

XMVECTOR ScreenPosComputer::ScreenPosVecCompute(XMFLOAT3 Pos_) {
	//���[���h���W�x�N�g���ɕϊ�
	XMVECTOR WorldPosVec = XMVectorSet(Pos_.x, Pos_.y, Pos_.z, 0.0f);

	//�J�����̏����擾
	XMFLOAT3 Eye    = CCamera::GetInstance()->GetEye();
	XMFLOAT3 LookAt = CCamera::GetInstance()->GetLookat();
	XMFLOAT3 Up     = CCamera::GetInstance()->GetUp();

	//�x�N�g���ɕϊ�
	XMVECTOR EyeVec    = XMVectorSet(Eye.x,    Eye.y,    Eye.z,    0.0f);
	XMVECTOR LookAtVec = XMVectorSet(LookAt.x, LookAt.y, LookAt.z, 0.0f);
	XMVECTOR UpVec     = XMVectorSet(Up.x,     Up.y,     Up.z,     0.0f);

	//�r���[�ƃv���W�F�N�V�����擾
	XMMATRIX View = XMMatrixLookAtLH(EyeVec, LookAtVec, UpVec);
	XMMATRIX Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_X / (FLOAT)SCREEN_Y, 0.1f, 1000.0f);
	//�r���[�ϊ��ƃv���W�F�N�V�����ϊ�
	WorldPosVec = XMVector3Transform(WorldPosVec, View);
	WorldPosVec = XMVector3Transform(WorldPosVec, Projection);

	XMFLOAT3 tempVec;
	XMStoreFloat3(&tempVec, WorldPosVec);

	// z�Ŋ����� -1 ~ 1 �͈̔͂Ɏ��߂�
	// �X�N���[�����W�ϊ�
	const XMVECTOR view_vec = XMVectorSet(tempVec.x / tempVec.z, tempVec.y / tempVec.z, tempVec.z, 1.0f);
	return XMVector3Transform(view_vec, m_ViewPortMatrix);
}

bool ScreenPosComputer::JudgeSiteIn(XMFLOAT3 Pos_) {
	//�T�C�g���Ɉ����̍��W�������Ă�����true��Ԃ�
	XMVECTOR PosVec_ = ScreenPosVecCompute(Pos_);
	float VecX = PosVec_.m128_f32[0];
	float VecY = PosVec_.m128_f32[1];
	float VecZ = PosVec_.m128_f32[2];

	if (VecZ <= 0) {
		return false;
	}
	if (VecX <= (SCREEN_X / 2) - SITE_SIZE || VecX >= (SCREEN_X / 2) + SITE_SIZE) {
		return false;
	}
	if (VecY <= (SCREEN_Y / 2) - SITE_SIZE || VecY >= (SCREEN_Y / 2) + SITE_SIZE) {
		return false;
	}

	return true;
}