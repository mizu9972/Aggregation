#include "CEffectiveObject.h"
#include "Draw2D.h"
#include "WindowSetting.h"
#include "CDirectxGraphics.h"
#include "Shader.h"
#include "CGameMain.h"
//フェードインの処理
struct ConstantFeedInColor {
	XMFLOAT4 Color;
};
CFeedIn::CFeedIn() {
	m_DrawTexture = new Draw2D;
}

void CFeedIn::Init(float MaxTime, XMFLOAT4 StartColor, XMFLOAT4 EndColor) {
	//初期化
	m_TimeCount = 0;//経過時間初期化
	m_FunctionMaxTime = MaxTime;//処理時間
	m_NowColor = StartColor;//開始色
	m_EndColor = EndColor;//終了色

	//加算色計算
	m_AddColor.x = (EndColor.x - StartColor.x) / (MaxTime * FPS);
	m_AddColor.y = (EndColor.y - StartColor.y) / (MaxTime * FPS);
	m_AddColor.z = (EndColor.z - StartColor.z) / (MaxTime * FPS);
	m_AddColor.w = (EndColor.w - StartColor.w) / (MaxTime * FPS);

	//テクスチャ初期化
	m_DrawTexture->Init(SCREEN_X / 2, SCREEN_Y / 2, 0, SCREEN_X, SCREEN_Y, m_NowColor, "assets/textures/FeedIn.png");

	//色変化させていく定数バッファ初期化
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
	//終了処理
	if (m_ConstantBuffer_Color != nullptr) {
		delete m_ConstantBuffer_Color;
	}
	m_DrawTexture->Uninit();
}

void CFeedIn::Update() {
	//更新
	m_TimeCount += 1.0f / FPS;//時間加算

	//色加算
	m_NowColor.x += m_AddColor.x;
	m_NowColor.y += m_AddColor.y;
	m_NowColor.z += m_AddColor.z;
	m_NowColor.w += m_AddColor.w;

	if (m_TimeCount >= m_FunctionMaxTime) {
		FunctionEnd();//終了処理
	}
}

void CFeedIn::Draw() {
	//描画

	//定数バッファ更新
	CDirectXGraphics::GetInstance()->GetImmediateContext()->UpdateSubresource(
		m_ConstantBuffer_Color,
		0,
		NULL,
		&m_NowColor,//反映色
		0,
		0
	);

	//定数バッファ適用
	CDirectXGraphics::GetInstance()->GetImmediateContext()->PSSetConstantBuffers(
		7,//レジスタ6
		1,
		&m_ConstantBuffer_Color
	);

	m_DrawTexture->Draw();//描画
}

void CFeedIn::FunctionEnd() {
	isActive = false;
	Notify();//監視クラスに通知
}