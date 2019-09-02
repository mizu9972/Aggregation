#include "FileLoader.h"

constexpr auto STAGE_MODEL_NAME = "";
constexpr auto AIRPLANE_MODEL_NAME = "assets/model/f1.x.dat";
void CFileLoader::Init() {
	//‰Šú‰»
	AirPlane = new CModel;
	AirPlane->Init(AIRPLANE_MODEL_NAME, "Shader/vs.fx", "Shader/ps.fx");
}

void CFileLoader::UnInit() {
	AirPlane->Uninit();
	delete AirPlane;
}

void CFileLoader::Draw() {

}

void CFileLoader::Draw(FileList File_) {

	switch (File_) {
	case FileList::AirPlane:
		AirPlane->Draw();
		break;
	}
}