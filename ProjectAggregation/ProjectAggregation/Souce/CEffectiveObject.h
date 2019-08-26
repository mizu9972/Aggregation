#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Observer.h"

using namespace DirectX;
//��ʃG�t�F�N�g���N���X
class CEffectiveObject: public Subject {
protected:
	bool isActive = false;
public:
	CEffectiveObject() { isActive = true; };
	virtual ~CEffectiveObject() {};
	virtual void Init() {};
	virtual void Init(float MaxTime, XMFLOAT4 StartColor, XMFLOAT4 EndColor) {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void FunctionEnd() { isActive = false; };
	bool GetActive() { return isActive; };
};

//�t�F�[�h�C���N���X-------------------------------
class Draw2D;
class CFeedIn : public CEffectiveObject{
private:

	ID3D11Buffer* m_ConstantBuffer_Color = nullptr;

	float m_TimeCount;
	float m_FunctionMaxTime;
	XMFLOAT4 m_NowColor;//���݂̐F
	XMFLOAT4 m_EndColor;//�I���F
	XMFLOAT4 m_AddColor;//���Z�F
	Draw2D* m_DrawTexture = nullptr;
public:
	CFeedIn();
	virtual void Init() {};
	virtual void Init(float MaxTime, XMFLOAT4 StartColor, XMFLOAT4 EndColor);
	virtual void UnInit();
	virtual void Update();
	virtual void Draw();
	virtual void FunctionEnd();//�t�F�[�h�C���I��

};

//-----------------------------------------------