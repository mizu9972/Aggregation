#include "CEffectiveObject.h"
#include "Draw2D.h"
#include "WindowSetting.h"
#include "CDirectxGraphics.h"
#include "Shader.h"
//フェードインの処理
struct ConstantFeedInColor {
	XMFLOAT4 Color;
};
CFeedIn::CFeedIn() {
	m_DrawTexture = new Draw2D;
}

void CFeedIn::Init(DWORD MaxTime, XMFLOAT4 StartColor, XMFLOAT4 EndColor) {
	m_TimeCount = 0;
	m_FunctionMaxTime = MaxTime;
	m_NowColor = StartColor;
	m_EndColor = EndColor;

	m_AddColor.x = (EndColor.x - StartColor.x) / (MaxTime * FPS);
	m_AddColor.y = (EndColor.y - StartColor.y) / (MaxTime * FPS);
	m_AddColor.z = (EndColor.z - StartColor.z) / (MaxTime * FPS);
	m_AddColor.w = (EndColor.w - StartColor.w) / (MaxTime * FPS);

	m_DrawTexture->Init(SCREEN_X / 2, SCREEN_Y / 2, 0, SCREEN_X, SCREEN_Y, m_NowColor, "assets/textures/Clear.png");

	bool sts = CreateConstantBuffer(
		CDirectXGraphics::GetInstance()->GetDXDevice(),
		sizeof(ConstantFeedInColor),
		&m_ConstantBuffer_Color
	);
	if (!sts) {
		return;
	}
}

void CFeedIn::UnInit() {
	if (m_ConstantBuffer_Color != nullptr) {
		delete m_ConstantBuffer_Color;
	}
}

void CFeedIn::Update() {
	m_TimeCount += 1 / FPS;

	m_NowColor.x += m_AddColor.x;
	m_NowColor.y += m_AddColor.y;
	m_NowColor.z += m_AddColor.z;
	m_NowColor.w += m_AddColor.w;

	if (m_TimeCount >= m_FunctionMaxTime) {
		FunctionEnd();
	}
}

void CFeedIn::Draw() {
	CDirectXGraphics::GetInstance()->GetImmediateContext()->UpdateSubresource(
		m_ConstantBuffer_Color,
		0,
		NULL,
		&m_NowColor,
		0,
		0
	);

	CDirectXGraphics::GetInstance()->GetImmediateContext()->PSSetConstantBuffers(
		6,
		1,
		&m_ConstantBuffer_Color
	);

	m_DrawTexture->Draw();
}

void CFeedIn::FunctionEnd() {

}