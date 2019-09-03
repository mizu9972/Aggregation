#include "Character.h"
#include "dx11mathutil.h"
#include "ObjectSetting.h"

//�v���C���[�E�G�L�����N�^�[�����t�@�C��
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

	//���݈ʒu��ۑ�
	OldPos.x = NowPos.x = m_Matrix._41;
	OldPos.y = NowPos.y = m_Matrix._42;
	OldPos.z = NowPos.z = m_Matrix._43;

	XMFLOAT4 qt;//�N�H�[�^�j�I��
	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	//�L�[���͂̂���ꍇ�����^�[������悤�ȓ�����
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


		//�s�񂩂�N�H�[�^�j�I���𐶐�
		DX11GetQtfromMatrix(m_Matrix, qt);

		XMFLOAT4 qtx, qty, qtz;//�N�H�[�^�j�I��

		//�w�莲��]�̃N�H�[�^�j�I���𐶐�
		DX11QtRotationAxis(qtx, axisX, m_Angle.x);
		DX11QtRotationAxis(qty, axisY, m_Angle.y);
		DX11QtRotationAxis(qtz, axisZ, m_Angle.z);

		//�N�H�[�^�j�I���𐳋K��

		//�N�H�[�^�j�I��������
		XMFLOAT4 tempqt1;
		DX11QtMul(tempqt1, qt, qtx);

		XMFLOAT4 tempqt2;
		DX11QtMul(tempqt2, qty, qtz);

		XMFLOAT4 tempqt3;
		DX11QtMul(tempqt3, tempqt1, tempqt2);

		//�N�H�[�^�j�I�����m�[�}���C�Y
		DX11QtNormalize(tempqt3, tempqt3);

		SetMatrixFromQt(tempqt3);
	}
	//�����O�i---------------------------------------
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
	//���W�Z�b�g
	m_Matrix._41 = Pos.x;
	m_Matrix._42 = Pos.y;
	m_Matrix._43 = Pos.z;
}

void Character::SetMatrixFromQt(XMFLOAT4 _Mat) {
	DX11MtxFromQt(m_Matrix, _Mat);
}