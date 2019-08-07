#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"Shader.h"
#include	"CCamera.h"
#include	"memory.h"
#include	"CLight.h"
#include	"DX11Settransform.h"

// �萔�o�b�t�@��`
ID3D11Buffer*       g_pConstantBuffer1 = nullptr;	// �R���X�^���g�o�b�t�@1

CDirectXGraphics	g_DXGrobj;			// �c�h�q�d�b�s�w�@�f�q�`�o�g�h�b�r�@�I�u�W�F�N�g		

// ���s������`
CLight				g_directionallight;

void DX11LightInit(DirectX::XMFLOAT4 lightpos) {
	g_directionallight.Init(CCamera::GetInstance()->GetEye(), lightpos);
}

void DX11LightUninit() {
	g_directionallight.Uninit();
}

void DX11LightUpdate(DirectX::XMFLOAT4 lpos) {
	g_directionallight.SetEyePos(CCamera::GetInstance()->GetEye());
	g_directionallight.SetLightPos(lpos);
	g_directionallight.Update();
}

CDirectXGraphics* GetDX11Obj() {
	return &g_DXGrobj;
}

ID3D11Device* GetDX11Device() {
	return g_DXGrobj.GetDXDevice();
}
ID3D11DeviceContext*	GetDX11DeviceContext() {
	return g_DXGrobj.GetImmediateContext();
}

bool DX11Init(HWND hwnd, int width, int height, bool fullscreen) {
	bool sts;

	sts = g_DXGrobj.Init(hwnd, width, height, fullscreen);
	if (!sts) {
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}

	// �A���t�@�u�����h�L��
	g_DXGrobj.TurnOnAlphaBlending();

	// �r�����s����������������������
	sts = DX11SetTransform::GetInstance()->Init();
	if (!sts) {
		MessageBox(NULL, "SetTransform error", "Error", MB_OK);
		return false;
	}

	return true;
}

void DX11Uninit() {

	DX11SetTransform::GetInstance()->Uninit();

	// �������
	g_DXGrobj.Exit();
}

void DX11BeforeRender(float ClearColor[]) {
	// �^�[�Q�b�g�o�b�t�@�N���A
	g_DXGrobj.GetImmediateContext()->ClearRenderTargetView(g_DXGrobj.GetRenderTargetView(), ClearColor);
	// Z�o�b�t�@�N���A
	g_DXGrobj.GetImmediateContext()->ClearDepthStencilView(g_DXGrobj.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11AfterRender() {

	// ��������
	g_DXGrobj.GetSwapChain()->Present(0, 0);
}

ID3D11RenderTargetView* DX11GetRenderTagetView() {
	return g_DXGrobj.GetRenderTargetView();
}

// Z�o�b�t�@������
void TurnOffZBuffer() {
	ID3D11RenderTargetView*  rtvtable[1];

	rtvtable[0] = DX11GetRenderTagetView();

	GetDX11DeviceContext()->OMSetRenderTargets(
		1,										// �^�[�Q�b�g
		rtvtable,								// �r���[�e�[�u��
		nullptr									// �[�x�o�b�t�@�Ȃ�
	);

}

// Z�o�b�t�@�L����
void TurnOnZBuffer() {
	ID3D11RenderTargetView*  rtvtable[1];

	rtvtable[0] = DX11GetRenderTagetView();

	g_DXGrobj.GetImmediateContext()->OMSetRenderTargets(
		1,										// �^�[�Q�b�g
		rtvtable,								// �r���[�e�[�u��
		g_DXGrobj.GetDepthStencilView()			// �[�x�o�b�t�@�Ȃ�
	);
}