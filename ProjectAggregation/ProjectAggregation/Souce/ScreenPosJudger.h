#pragma once
#include "dx11mathutil.h"
#include "WindowSetting.h"

using namespace DirectX;

//画面上の描画位置を計算するクラス
//プレイヤーの攻撃の判定に利用する
class ScreenPosComputer {
private:
	XMVECTOR m_EyeVec;
	XMVECTOR m_UpVec;
	XMVECTOR m_LookAtVec;

	//ビューポート変換行列
	const XMMATRIX m_ViewPortMatrix = {
		SCREEN_X / 2,			   0, 0, 0,
		           0, - SCREEN_Y / 2, 0, 0,
				   0,			   0, 1, 0,
		SCREEN_X / 2,	SCREEN_Y / 2, 0, 1
	};

	ScreenPosComputer();
	~ScreenPosComputer() = default;

	XMVECTOR ScreenPosVecCompute(XMFLOAT3 Pos_);
public:
	ScreenPosComputer(const ScreenPosComputer&) = delete;
	ScreenPosComputer(ScreenPosComputer&&) = delete;
	ScreenPosComputer operator = (const ScreenPosComputer&) = delete;
	ScreenPosComputer operator = (ScreenPosComputer&&) = delete;

	static ScreenPosComputer* GetInstance() {
		static ScreenPosComputer instance;
		return &instance;
	}

	bool JudgeSiteIn(XMFLOAT3 Pos_);
};