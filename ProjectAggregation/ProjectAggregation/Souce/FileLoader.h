#pragma once

#include "CModel.h"
//外部から読み込んだデータを一括管理するクラス
class CFileLoader {
private:
	CModel* SkyDome;
	CModel* AirPlane;

	CFileLoader() {};
	~CFileLoader() = default;
public:
	CFileLoader(const CFileLoader&) = delete;
	CFileLoader(CFileLoader&&) = delete;
	CFileLoader operator = (const CFileLoader&) = delete;
	CFileLoader operator = (CFileLoader&&) = delete;

	static CFileLoader* GetInstance() {
		static CFileLoader instance;
		return &instance;
	}

	enum class FileList {
		AirPlane,
		SkyDome,
	};

	void Init();//初期化
	void UnInit();

	void Draw();//動かないオブジェクトの描画
	void Draw(FileList File_);
};