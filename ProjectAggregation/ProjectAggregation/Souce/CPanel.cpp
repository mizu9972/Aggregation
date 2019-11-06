#include "CPanel.h"

#define STS_ifERROR_FUNCTION 	if (!sts) { MessageBox(NULL, "AnyFunction is Error", "Error", MB_OK); return;}

void CPanel::Init(Vertex4* pos, const char* filename, const char* vsfilename, const char* psfilename) {
	//����������

	//�f�o�C�X�擾
	ID3D11Device* device;

	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	//���_���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	unsigned int ElementNum = ARRAYSIZE(layout);//�G�������g��

	//���_�V�F�[�_�[�I�u�W�F�N�g�쐬
	bool sts = CreateVertexShader(
		device,
		vsfilename,
		"main",
		"vs_4_0",
		layout,
		ElementNum,
		&m_VertexShader,
		&m_VertexLayout);
	STS_ifERROR_FUNCTION;

	//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�쐬
	sts = CreatePixelShader(
		device,
		psfilename,
		"main",
		"ps_4_0",
		&m_PixelShader);
	STS_ifERROR_FUNCTION;

	//���_�o�b�t�@�쐬
	sts = CreateVertexBuffer(
		device,
		sizeof(pos),
		4,
		pos,
		&m_VertextBuffer
	);
	STS_ifERROR_FUNCTION;

	return;
}

void CPanel::Draw() {
	//�`��

	//�f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
}

void CPanel::UnInit() {
	if (m_VertextBuffer != nullptr) {
		m_VertextBuffer->Release();
		m_VertextBuffer = nullptr;
	}

	if (m_VertexShader != nullptr) {
		m_VertexShader->Release();
		m_VertexShader = nullptr;
	}

	if (m_PixelShader != nullptr) {
		m_PixelShader->Release();
		m_PixelShader = nullptr;
	}

	if (m_VertexLayout != nullptr) {
		m_VertexLayout->Release();
		m_VertexLayout = nullptr;
	}
}