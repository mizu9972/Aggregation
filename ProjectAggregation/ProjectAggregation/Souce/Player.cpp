#include "Character.h"

void Player::Init() {
	Character::Init();

}

void Player::UnInit() {

}

void Player::Update() {

	Character::Move(true);//移動(キー操作があったら引数をtrue)

	// 角度設定リセット
	m_Angle.x = 0.0f;
	m_Angle.y = 0.0f;
}

void Player::Draw() {

}

//キー入力に対する処理
void Player::Up() {
	//上移動
	m_Angle.x = 1.0f;
	m_Angle.y = 0.0f;
}

void Player::Down() {
	//下移動
	m_Angle.x = -1.0f;
	m_Angle.y = 0.0f;
}

void Player::Right() {
	//右移動
	m_Angle.x = 0.0f;
	m_Angle.y = 1.0f;
}

void Player::Left() {
	//左移動
	m_Angle.x = 0.0f;
	m_Angle.y = -1.0f;
}