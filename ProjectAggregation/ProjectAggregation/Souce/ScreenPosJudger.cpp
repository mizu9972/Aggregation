#include "ScreenPosJudger.h"
#include "CCamera.h"
#include "ObjectSetting.h"
#include "DX11Settransform.h"

ScreenPosComputer::ScreenPosComputer() {
	
}

XMVECTOR ScreenPosComputer::ScreenPosVecCompute(XMFLOAT3 Pos_) {
	//ワールド座標ベクトルに変換
	XMVECTOR WorldPosVec = XMVectorSet(Pos_.x, Pos_.y, Pos_.z, 0.0f);

	//カメラの情報を取得
	XMFLOAT3 Eye    = CCamera::GetInstance()->GetEye();
	XMFLOAT3 LookAt = CCamera::GetInstance()->GetLookat();
	XMFLOAT3 Up     = CCamera::GetInstance()->GetUp();

	//ベクトルに変換
	XMVECTOR EyeVec    = XMVectorSet(Eye.x,    Eye.y,    Eye.z,    0.0f);
	XMVECTOR LookAtVec = XMVectorSet(LookAt.x, LookAt.y, LookAt.z, 0.0f);
	XMVECTOR UpVec     = XMVectorSet(Up.x,     Up.y,     Up.z,     0.0f);

	//ビューとプロジェクション取得
	XMMATRIX View = XMMatrixLookAtLH(EyeVec, LookAtVec, UpVec);
	XMMATRIX Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_X / (FLOAT)SCREEN_Y, 0.1f, 1000.0f);
	//ビュー変換とプロジェクション変換
	WorldPosVec = XMVector3Transform(WorldPosVec, View);
	WorldPosVec = XMVector3Transform(WorldPosVec, Projection);

	XMFLOAT3 tempVec;
	XMStoreFloat3(&tempVec, WorldPosVec);

	// zで割って -1 ~ 1 の範囲に収める
	// スクリーン座標変換
	const XMVECTOR view_vec = XMVectorSet(tempVec.x / tempVec.z, tempVec.y / tempVec.z, tempVec.z, 1.0f);
	return XMVector3Transform(view_vec, m_ViewPortMatrix);
}

bool ScreenPosComputer::JudgeSiteIn(XMFLOAT3 Pos_) {
	//サイト内に引数の座標が入っていたらtrueを返す
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