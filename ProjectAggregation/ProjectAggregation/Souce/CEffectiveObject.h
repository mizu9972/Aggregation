#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
//画面エフェクト基底クラス
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

//フェードインクラス-------------------------------
class Draw2D;
class CFeedIn : public CEffectiveObject{
private:

	ID3D11Buffer* m_ConstantBuffer_Color = nullptr;

	DWORD m_TimeCount;
	DWORD m_FunctionMaxTime;
	XMFLOAT4 m_NowColor;//現在の色
	XMFLOAT4 m_EndColor;//終了色
	XMFLOAT4 m_AddColor;//加算色
	Draw2D* m_DrawTexture = nullptr;
public:
	CFeedIn();
	virtual void Init() {};
	virtual void Init(DWORD MaxTime, XMFLOAT4 StartColor, XMFLOAT4 EndColor);
	virtual void UnInit();
	virtual void Update();
	virtual void Draw();
	virtual void FunctionEnd();//フェードイン終了
};

//-----------------------------------------------