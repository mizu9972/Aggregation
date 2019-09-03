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
	Character::Move(true);//常にtrue
}

void Enemy::RandomMove() {
	//オートパイロット時の動作を決定する

	if (m_ActionContinueTime <= 0) {

		int SetActionNum = rand() % Enemy::ACTION_NUM - 1;//行動決定
		int SetActionTime = rand() % (RANDOM_MOVE_CONTINUE_TIME_MAX + RANDOM_MOVE_CONTINUE_TIME_MIN) + RANDOM_MOVE_CONTINUE_TIME_MIN;//行動継続時間決定

		//適用
		m_Action = SetActionNum;
		m_ActionContinueTime = SetActionTime * FPS;
	}
	m_ActionContinueTime -= 1;

	//移動方向設定
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
	//描画

	//ワールド座標変換後描画する
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_Matrix);
	CFileLoader::GetInstance()->Draw(CFileLoader::FileList::AirPlane);
}

void Enemy::HitFunction() {
	//被弾時処理
	Character::HitFunction();
	
}