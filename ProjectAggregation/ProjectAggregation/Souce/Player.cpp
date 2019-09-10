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

	Character::Move(true);//移動(キー操作があったら引数をtrue)

	//カメラ操作
	CameraMove();

	// 角度設定リセット
	m_Angle.x = 0.0f;
	m_Angle.y = 0.0f;
	m_Angle.z = 0.0f;
}

void Player::CameraMove() {
	//カメラ操作
	XMFLOAT3 Eye_;
	XMFLOAT3 Up_;
	XMFLOAT3 LookAt_;
	
	//視点をやや前方に
	LookAt_.x = m_Matrix._41 + m_Matrix._31 * 5.0f;
	LookAt_.y = m_Matrix._42 + m_Matrix._32 * 5.0f;
	LookAt_.z = m_Matrix._43 + m_Matrix._33 * 5.0f;

	//位置・傾きはプレイヤーにあわせる
	//カメラはずらさずモデルをずらす
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