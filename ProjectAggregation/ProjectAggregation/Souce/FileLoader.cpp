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

	//初期化

	//モデルデータの登録・初期化
	m_ModelCollection["AirPlane"] = new CModel();
	m_ModelCollection["SkyDome"]  = new CModel();
	m_ModelCollection["CockPit"]  = new CModel();
	m_ModelCollection["Panel"]    = new CModel();

	//									モデルディレクトリ       頂点シェーダー            ピクセルシェーダー  
	m_ModelCollection["AirPlane"]->Init(AIRPLANE_MODEL_NAME, "Shader/vs.fx",        "Shader/ps.fx");
	m_ModelCollection["SkyDome"]->Init( SKYDOME_MODEL_NAME,  "Shader/vsskydome.fx", "Shader/psskydome.fx");
	m_ModelCollection["CockPit"]->Init( COCKPIT_MODEL_NAME,  "Shader/vs.fx",        "Shader/psCockPit.fx");
	m_ModelCollection["Panel"]->Init(   PANEL_MODEL_NAME,    "Shader/vsskydome.fx", "Shader/psskydome.fx");

	ConstantBufferManager::GetInstance()->Init();

	//スカイドームに重ねる画像のシェーダーリソースビュー作成
	m_StarsSRV = new ID3D11ShaderResourceView*;
	bool sts = CreatetSRVfromTGAFile("assets/textures/alonestar.tga", CDirectXGraphics::GetInstance()->GetDXDevice(), m_StarsSRV);
	ifERROR_FUNCTION(sts)


	//重ねる画像をシェーダーにセット
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
	//終了処理

	//モデルリスト終了処理
	for (auto key = m_ModelCollection.begin(); key != m_ModelCollection.end(); key++) {
		key->second->Uninit();
	}
	if (m_StarsSRV != nullptr) {
		delete m_StarsSRV;
	}

}

void CFileLoader::Draw(FileList File_) {
	//描画するものをリストから指定して描画する
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