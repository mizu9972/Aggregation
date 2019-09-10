#include "FileLoader.h"

constexpr auto STAGE_MODEL_NAME = "";
constexpr auto AIRPLANE_MODEL_NAME = "assets/model/f1.x.dat";
constexpr auto SKYDOME_MODEL_NAME  = "assets/model/skydome.x.dat";
constexpr auto COCKPIT_MODEL_NAME  = "assets/model/CockPit.x.dat";

void CFileLoader::Init() {
	//������
	AirPlane = new CModel;
	SkyDome = new CModel;
	CockPit = new CModel;

	AirPlane->Init(AIRPLANE_MODEL_NAME, "Shader/vs.fx", "Shader/ps.fx");
	SkyDome->Init(SKYDOME_MODEL_NAME, "Shader/vs.fx", "Shader/psskydome.fx");
	CockPit->Init(COCKPIT_MODEL_NAME, "Shader/vs.fx", "Shader/psCockPit.fx");
}

void CFileLoader::UnInit() {
	AirPlane->Uninit();
	SkyDome->Uninit();
	CockPit->Uninit();

	delete AirPlane;
	delete SkyDome;
	delete CockPit;
}

void CFileLoader::Draw() {
	//�����w��Ȃ��̏ꍇ
	//�Œ�I�u�W�F�N�g�����ׂĕ`��
	SkyDome->Draw();
}

void CFileLoader::Draw(FileList File_) {
	//�`�悷����̂����X�g����w�肵�ĕ`�悷��
	switch (File_) {
	case FileList::AirPlane:
		AirPlane->Draw();
		break;

	case FileList::SkyDome:
		SkyDome->Draw();
		break;

	case FileList::CockPit:
		CockPit->Draw();
	}

}