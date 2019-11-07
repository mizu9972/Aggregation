#include "FileLoader.h"
#include "CDirectxGraphics.h"
#include "Shader.h"
#include "ConstantBuffer.h"

#define ifERROR_FUNCTION(sts) 	if (!sts) { MessageBox(NULL, "AnyFunction is Error", "Error", MB_OK); return;}

constexpr auto AIRPLANE_MODEL_NAME = "assets/model/f1.x.dat";
constexpr auto SKYDOME_MODEL_NAME  = "assets/model/skydome.x.dat";
constexpr auto COCKPIT_MODEL_NAME  = "assets/model/CockPit.x.dat";
constexpr auto PANEL_MODEL_NAME	   = "assets/model/panel02.x.dat";

void CFileLoader::Init() {

	//������

	//���f���f�[�^�̓o�^�E������
	m_ModelCollection["AirPlane"] = new CModel();
	m_ModelCollection["SkyDome"]  = new CModel();
	m_ModelCollection["CockPit"]  = new CModel();
	m_ModelCollection["Panel"]    = new CModel();

	//									���f���f�B���N�g��       ���_�V�F�[�_�[            �s�N�Z���V�F�[�_�[  
	m_ModelCollection["AirPlane"]->Init(AIRPLANE_MODEL_NAME, "Shader/vs.fx",        "Shader/ps.fx");
	m_ModelCollection["SkyDome"]->Init( SKYDOME_MODEL_NAME,  "Shader/vsskydome.fx", "Shader/psskydome.fx");
	m_ModelCollection["CockPit"]->Init( COCKPIT_MODEL_NAME,  "Shader/vs.fx",        "Shader/psCockPit.fx");
	m_ModelCollection["Panel"]->Init(   PANEL_MODEL_NAME,    "Shader/vsskydome.fx", "Shader/psskydome.fx");

	ConstantBufferManager::GetInstance()->Init();

	//�X�J�C�h�[���ɏd�˂�摜�̃V�F�[�_�[���\�[�X�r���[�쐬
	m_StarsSRV = new ID3D11ShaderResourceView*;
	bool sts = CreatetSRVfromTGAFile("assets/textures/alonestar.tga", CDirectXGraphics::GetInstance()->GetDXDevice(), m_StarsSRV);
	ifERROR_FUNCTION(sts)


	//�d�˂�摜���V�F�[�_�[�ɃZ�b�g
	CDirectXGraphics::GetInstance()->GetImmediateContext()->PSSetShaderResources(
		1,
		1,
		m_StarsSRV
	);
	CDirectXGraphics::GetInstance()->GetImmediateContext()->VSSetShaderResources(
		1,
		1,
		m_StarsSRV
	);

}

void CFileLoader::UnInit() {
	//�I������

	//���f�����X�g�I������
	for (auto key = m_ModelCollection.begin(); key != m_ModelCollection.end(); key++) {
		key->second->Uninit();
	}
	if (m_StarsSRV != nullptr) {
		delete m_StarsSRV;
	}

}

void CFileLoader::Draw(FileList File_) {
	//�`�悷����̂����X�g����w�肵�ĕ`�悷��
	switch (File_) {
	case FileList::AirPlane:
		//m_ModelCollection["AirPlane"]->Draw();
		break;

	case FileList::SkyDome:

		ConstantBufferManager::GetInstance()->UpdateConstantBuffer(ConstantBufferManager::RegistarList::Time);

		m_ModelCollection["SkyDome"]->Draw();


		break;

	case FileList::CockPit:
		m_ModelCollection["CockPit"]->Draw();
		m_ModelCollection["Panel"]->Draw();
		break;
	}

}