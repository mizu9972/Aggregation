#pragma once

#include <DirectXMath.h>
#include "Shader.h"
#include "CDirectxGraphics.h"

using namespace DirectX;

struct Vertex4 {
	XMFLOAT4 Pos;
};

class CPanel {
private:
	ID3D11Buffer*		m_VertextBuffer      = nullptr;
	ID3D11VertexShader* m_VertexShader       = nullptr;
	ID3D11PixelShader*  m_PixelShader        = nullptr;
	ID3D11InputLayout*  m_VertexLayout       = nullptr;

public:
	void Init(Vertex4* pos,const char* filename,const char* vsfilename,const char* psfilename);
	void Draw();
	void UnInit();
};