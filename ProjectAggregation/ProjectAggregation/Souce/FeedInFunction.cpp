#include "CEffectiveObject.h"
#include "Draw2D.h"
#include "WindowSetting.h"
#include "CDirectxGraphics.h"
#include "Shader.h"
#include "CGameMain.h"
//�t�F�[�h�C���̏���
struct ConstantFeedInColor {
	XMFLOAT4 Color;
};
CFeedIn::CFeedIn() {
	m_DrawTexture = new Draw2D;
}

void CFeedIn::Init(float MaxTime, XMFLOAT4 StartColor, XMFLOAT4 EndColor) {
	//������
	m_TimeCount = 0;//�o�ߎ��ԏ�����
	m_FunctionMaxTime = MaxTime;//��������
	m_NowColor = StartColor;//�J�n�F
	m_EndColor = EndColor;//�I���F

	//���Z�F�v�Z
	m_AddColor.x = (EndColor.x - StartColor.x) / (MaxTime * FPS);
	m_AddColor.y = (EndColor.y - StartColor.y) / (MaxTime * FPS);
	m_AddColor.z = (EndColor.z - StartColor.z) / (MaxTime * FPS);
	m_AddColor.w = (EndColor.w - StartColor.w) / (MaxTime * FPS);

	//�e�N�X�`��������
	m_DrawTexture->Init(SCREEN_X / 2, SCREEN_Y / 2, 0, SCREEN_X, SCREEN_Y, m_NowColor, "assets/textures/FeedIn.png");

	//�F�ω������Ă����萔�o�b�t�@������
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
	//�I������
	if (m_ConstantBuffer_Color != nullptr) {
		delete m_ConstantBuffer_Color;
	}
	m_DrawTexture->Uninit();
}

void CFeedIn::Update() {
	//�X�V
	m_TimeCount += 1.0f / FPS;//���ԉ��Z

	//�F���Z
	m_NowColor.x += m_AddColor.x;
	m_NowColor.y += m_AddColor.y;
	m_NowColor.z += m_AddColor.z;
	m_NowColor.w += m_AddColor.w;

	if (m_TimeCount >= m_FunctionMaxTime) {
		FunctionEnd();//�I������
	}
}

void CFeedIn::Draw() {
	//�`��

	//�萔�o�b�t�@�X�V
	CDirectXGraphics::GetInstance()->GetImmediateContext()->UpdateSubresource(
		m_ConstantBuffer_Color,
		0,
		NULL,
		&m_NowColor,//���f�F
		0,
		0
	);

	//�萔�o�b�t�@�K�p
	CDirectXGraphics::GetInstance()->GetImmediateContext()->PSSetConstantBuffers(
		7,//���W�X�^6
		1,
		&m_ConstantBuffer_Color
	);

	m_DrawTexture->Draw();//�`��
}

void CFeedIn::FunctionEnd() {
	isActive = false;
	Notify();//�Ď��N���X�ɒʒm
}