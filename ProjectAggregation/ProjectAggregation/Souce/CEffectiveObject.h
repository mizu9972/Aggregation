#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
//��ʃG�t�F�N�g���N���X
class CEffectiveObject {
private:
public:
	CEffectiveObject() {};
	virtual ~CEffectiveObject() {};
	virtual void Init() {};
	virtual void Init(DWORD MaxTime, XMFLOAT4 StartColor, XMFLOAT4 EndColor) {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void FunctionEnd() {};
};

//�t�F�[�h�C���N���X-------------------------------
class Draw2D;
class CFeedIn : public CEffectiveObject{
private:

	ID3D11Buffer* m_ConstantBuffer_Color = nullptr;

	DWORD m_TimeCount;
	DWORD m_FunctionMaxTime;
	XMFLOAT4 m_NowColor;//���݂̐F
	XMFLOAT4 m_EndColor;//�I���F
	XMFLOAT4 m_AddColor;//���Z�F
	Draw2D* m_DrawTexture = nullptr;
public:
	CFeedIn();
	virtual void Init() {};
	virtual void Init(DWORD MaxTime, XMFLOAT4 StartColor, XMFLOAT4 EndColor);
	virtual void UnInit();
	virtual void Update();
	virtual void Draw();
	virtual void FunctionEnd();//�t�F�[�h�C���I��
};

//-----------------------------------------------