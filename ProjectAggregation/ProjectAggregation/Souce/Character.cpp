#include "Character.h"
#include "dx11mathutil.h"
#include "ObjectSetting.h"

//プレイヤー・敵キャラクター処理ファイル
void Character::Init() {
	DX11MatrixIdentity(m_Matrix);

	m_HP      = CHARACTER_HP;
	m_Speed   = CHARACTER_SPEED;

	m_Angle.x = 0;
	m_Angle.y = 0;
	m_Angle.z = 0;
}

void Character::Move(bool KeyInput) {
	XMFLOAT3 OldPos;
	XMFLOAT3 NowPos;

	//現在位置を保存
	OldPos.x = NowPos.x = m_Matrix._41;
	OldPos.y = NowPos.y = m_Matrix._42;
	OldPos.z = NowPos.z = m_Matrix._43;

	XMFLOAT4 qt;//クォータニオン
	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	//キー入力のある場合だけターンするような動きに
	if (KeyInput) {
		axisX.x = m_Matrix._11;
		axisX.y = m_Matrix._12;
		axisX.z = m_Matrix._13;

		axisY.x = m_Matrix._21;
		axisY.y = m_Matrix._22;
		axisY.z = m_Matrix._23;

		axisZ.x = m_Matrix._31;
		axisZ.y = m_Matrix._32;
		axisZ.z = m_Matrix._33;


		//行列からクォータニオンを生成
		DX11GetQtfromMatrix(m_Matrix, qt);

		XMFLOAT4 qtx, qty, qtz;//クォータニオン

		//指定軸回転のクォータニオンを生成
		DX11QtRotationAxis(qtx, axisX, m_Angle.x);
		DX11QtRotationAxis(qty, axisY, m_Angle.y);
		DX11QtRotationAxis(qtz, axisZ, m_Angle.z);

		//クォータニオンを正規化

		//クォータニオンを合成
		XMFLOAT4 tempqt1;
		DX11QtMul(tempqt1, qt, qtx);

		XMFLOAT4 tempqt2;
		DX11QtMul(tempqt2, qty, qtz);

		XMFLOAT4 tempqt3;
		DX11QtMul(tempqt3, tempqt1, tempqt2);

		//クォータニオンをノーマライズ
		DX11QtNormalize(tempqt3, tempqt3);

		SetMatrixFromQt(tempqt3);
	}
	//自動前進---------------------------------------
	NowPos.x += m_Matrix._31 * m_Speed;
	NowPos.y += m_Matrix._32 * m_Speed;
	NowPos.z += m_Matrix._33 * m_Speed;

	m_Matrix._41 = NowPos.x;
	m_Matrix._42 = NowPos.y;
	m_Matrix._43 = NowPos.z;
	//----------------------------------------------


}

void Character::HitFunction() {
	m_HP -= 1;

	if (m_HP <= 0) {
		Notify(this);
	}
}

void Character::SetTransform(XMFLOAT3 Pos) {
	//座標セット
	m_Matrix._41 = Pos.x;
	m_Matrix._42 = Pos.y;
	m_Matrix._43 = Pos.z;
}

void Character::SetMatrixFromQt(XMFLOAT4 _Mat) {
	DX11MtxFromQt(m_Matrix, _Mat);
}