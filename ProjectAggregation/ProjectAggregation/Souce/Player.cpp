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

	//角度初期化関数
	auto AngleReset = [](XMFLOAT3& Angle_) { 
		Angle_.x = 0.0f;
		Angle_.y = 0.0f;
		Angle_.z = 0.0f;
	};


	Character::Move(true);//移動(キー操作があったら引数をtrue)

	//カメラ操作
	CameraMove();

	// 角度設定リセット
	AngleReset(m_Angle);
}

void Player::CameraMove() {
	//カメラの操作
	
	//角度をラジアンに変換関数
	auto ToRadian = [](float Angle) { return (Angle * XM_PI) / 180.0; };
	
	XMFLOAT3 Eye_;
	XMFLOAT3 Up_;
	XMFLOAT3 LookAt_;

	
	
	//位置・傾きはプレイヤーにあわせる
	//カメラはずらさずモデルをずらす
	Eye_.x = m_Matrix._41;
	Eye_.y = m_Matrix._42;
	Eye_.z = m_Matrix._43;
	Up_.x  = m_Matrix._21;
	Up_.y  = m_Matrix._22;
	Up_.z  = m_Matrix._23;

	//回転座標取得
	float HorizonAgnle = CCamera::GetInstance()->GetHorizonAngle(&Eye_, &LookAt_);
	float VerticalAngle = CCamera::GetInstance()->GetVerticalAngle(&Eye_, &LookAt_);

	//視点移動量加算
	HorizonAgnle += m_CameraAngle.x;
	VerticalAngle += m_CameraAngle.y;
	XMFLOAT3 SetAngle = { HorizonAgnle,VerticalAngle,0.0f };

	float r = 5.0f + sinf(ToRadian(VerticalAngle));
	//視点をやや前方に
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
	//描画
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_Matrix);
	CFileLoader::GetInstance()->Draw(CFileLoader::FileList::CockPit);
}

//キー入力に対する処理
void Player::Up() {
	//上移動
	m_Angle.x += 1.0f;
	m_Angle.y += 0.0f;

}

void Player::Down() {
	//下移動
	m_Angle.x += -1.0f;
	m_Angle.y += 0.0f;

}

void Player::Right() {
	//右移動
	m_Angle.x += 0.0f;
	m_Angle.y += 1.0f;

}

void Player::Left() {
	//左移動
	m_Angle.x += 0.0f;
	m_Angle.y += -1.0f;

}

void Player::R_Turn() {
	//右回転
	m_Angle.z -= 1.0f;
}

void Player::L_Turn() {
	//左回転
	m_Angle.z += 1.0f;
}

void Player::Act() {
	//アクション
	//攻撃

	//当たり判定する処理呼び出し
	CGameMain::GetInstance()->getNowScene()->ObjectHitJudge();
}

//カメラ操作
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