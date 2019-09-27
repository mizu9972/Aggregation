#include "FileLoader.h"
#include "CDirectxGraphics.h"
#include "Shader.h"

#define STS_ifERROR_FUNCTION 	if (!sts) { MessageBox(NULL, "AnyFunction is Error", "Error", MB_OK); return;}

constexpr auto STAGE_MODEL_NAME = "";
constexpr auto AIRPLANE_MODEL_NAME = "assets/model/f1.x.dat";
constexpr auto SKYDOME_MODEL_NAME  = "assets/model/skydome.x.dat";
constexpr auto COCKPIT_MODEL_NAME  = "assets/model/CockPit.x.dat";

void CFileLoader::Init() {

	//初期化
	AirPlane = new CModel;
	SkyDome = new CModel;
	CockPit = new CModel;

	AirPlane->Init(AIRPLANE_MODEL_NAME, "Shader/vs.fx", "Shader/ps.fx");
	SkyDome->Init(SKYDOME_MODEL_NAME, "Shader/vs.fx", "Shader/psskydome.fx");
	CockPit->Init(COCKPIT_MODEL_NAME, "Shader/vs.fx", "Shader/psCockPit.fx");

	//時間を流す定数バッファ作成
	bool sts = CreateConstantBuffer(
		CDirectXGraphics::GetInstance()->GetDXDevice(),
		sizeof(ConstantBufferTime),
		&m_ConstantBufferTime
	);
	STS_ifERROR_FUNCTION
	m_CurrentTime.Padding = 0;

	//重ねる画像のシェーダーリソースビュー作成
	m_StarsSRV = new ID3D11ShaderResourceView*;
	sts = CreatetSRVfromTGAFile("assets/textures/Stars.tga", CDirectXGraphics::GetInstance()->GetDXDevice(), m_StarsSRV);
	STS_ifERROR_FUNCTION

}

void CFileLoader::UnInit() {
	//終了処理
	AirPlane->Uninit();
	SkyDome->Uninit();
	CockPit->Uninit();

	if (m_StarsSRV != nullptr) {
		delete m_StarsSRV;
	}

	delete AirPlane;
	delete SkyDome;
	delete CockPit;
}

void CFileLoader::Draw() {
	//引数指定なしの場合
	//固定オブジェクトをすべて描画
	SkyDome->Draw();
}

void CFileLoader::Draw(FileList File_) {
	//描画するものをリストから指定して描画する
	switch (File_) {
	case FileList::AirPlane:
		AirPlane->Draw();
		break;

	case FileList::SkyDome:
		//現在時間をピクセルシェーダーに流す
		m_CurrentTime.iTime = static_cast<double>(timeGetTime());


		CDirectXGraphics::GetInstance()->GetImmediateContext()->UpdateSubresource(
			m_ConstantBufferTime,
			0,
			NULL,
			&m_CurrentTime,
			0,
			0
		);

		CDirectXGraphics::GetInstance()->GetImmediateContext()->PSSetConstantBuffers(
			6,
			1,
			&m_ConstantBufferTime
		);

		//重ねる画像をピクセルシェーダーにセット
		CDirectXGraphics::GetInstance()->GetImmediateContext()->PSSetShaderResources(
			1,
			1,
			m_StarsSRV
		);

		SkyDome->Draw();//スカイドーム描画
		break;

	case FileList::CockPit:
		CockPit->Draw();
		break;
	}

}