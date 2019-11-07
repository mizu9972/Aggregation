#pragma once

#include <DirectXMath.h>
#include "Observer.h"
#include "Command.h"
#include "Interface.h"

using namespace DirectX;
class Character : public IHit , public Subject{
private:
protected:
	int m_HP;
	float m_Speed;
	float m_MaxSpeed;
	XMFLOAT4X4 m_Matrix;

	//座標情報
	XMFLOAT3 m_Angle;
	XMFLOAT3 m_Trans;
	XMFLOAT3 m_Zpos;

public:
	virtual ~Character() {};
	virtual void Init();
	virtual void Update() {};
	void Move(bool KeyInput);
	virtual void Draw() {};
	virtual void UnInit() {};

	virtual void HitFunction();

	void SetTransform(XMFLOAT3 Pos);
	void SetMatrixFromQt(XMFLOAT4 _Mat);

	XMFLOAT4X4 GetMatrix() { return m_Matrix; };
	XMFLOAT3 GetPos() { return XMFLOAT3(m_Matrix._41, m_Matrix._42, m_Matrix._43); };

};


class Player : public Character {
private:
	XMFLOAT3 m_CameraAngle;
	XMFLOAT4X4 m_CameraMatrix;
	void CameraMove();//カメラを操作
public:
	//オーバーライド-----------
	//Characterクラス
	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void UnInit();
	//----------------------


	//キー入力に対する操作
	void Playing();

    void Up();
    void Down();
    void Right();
    void Left();
    void R_Turn();
    void L_Turn();
    void Act();
	void Accel();
	void Brake();
    void SubUp();
    void SubDown();
    void SubRight();
	void SubLeft();


};

class Enemy : public Character{
private:
	//行動リスト
	typedef enum {
		UP = 0,
		DOWN,
		RIGHT,
		LEFT,
		STRAIGHT,

		ACTION_NUM,//アクション数
	}me_ActionList;

	BYTE m_Action;//現在の行動
	int m_ActionContinueTime = 0;//アクション継続時間


	void RandomMove();
public:

	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void UnInit();

	virtual void HitFunction();

};