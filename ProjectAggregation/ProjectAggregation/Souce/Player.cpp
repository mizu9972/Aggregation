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

	//Character::Move(true);//移動(キー操作があったら引数をtrue)
	CCamera::GetInstance()->SetLookat(LookAtPoint);
	CCamera::GetInstance()->CreateCameraMatrix();
	// 角度設定リセット
	m_Angle.x = 0.0f;
	m_Angle.y = 0.0f;
}

void Player::Draw() {
	//描画
}

//キー入力に対する処理
void Player::Up() {
	//上移動
	m_Angle.x = 1.0f;
	m_Angle.y = 0.0f;

	LookAtPoint.y += 0.1f;
}

void Player::Down() {
	//下移動
	m_Angle.x = -1.0f;
	m_Angle.y = 0.0f;

	LookAtPoint.y -= 0.1f;
}

void Player::Right() {
	//右移動
	m_Angle.x = 0.0f;
	m_Angle.y = 1.0f;

	LookAtPoint.x += 0.1f;
}

void Player::Left() {
	//左移動
	m_Angle.x = 0.0f;
	m_Angle.y = -1.0f;

	LookAtPoint.x -= 0.1f;
}

void Player::Act() {
	//アクション
	//攻撃

	//当たり判定する処理呼び出し
	CGameMain::GetInstance()->getNowScene()->ObjectHitJudge();
}