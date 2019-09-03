#include "FileLoader.h"

constexpr auto STAGE_MODEL_NAME = "";
constexpr auto AIRPLANE_MODEL_NAME = "assets/model/f1.x.dat";
constexpr auto SKYDOME_MODEL_NAME = "assets/model/skydome.x.dat";

void CFileLoader::Init() {
	//‰Šú‰»
	AirPlane = new CModel;
	SkyDome = new CModel;
	AirPlane->Init(AIRPLANE_MODEL_NAME, "Shader/vs.fx", "Shader/ps.fx");
	SkyDome->Init(SKYDOME_MODEL_NAME, "Shader/vs.fx", "Shader/psskydome.fx");
}

void CFileLoader::UnInit() {
	AirPlane->Uninit();
	SkyDome->Uninit();

	delete AirPlane;
	delete SkyDome;
}

void CFileLoader::Draw() {

}

void CFileLoader::Draw(FileList File_) {

	switch (File_) {
	case FileList::AirPlane:
		AirPlane->Draw();
		break;

	case FileList::SkyDome:
		SkyDome->Draw();
		break;
	}

}