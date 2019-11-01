#pragma once

#include <unordered_map>
#include "CModel.h"
#include "Shader.h"

//外部から読み込んだデータを一括管理するクラス
class CFileLoader {
private:
	std::unordered_map<std::string, CModel*> m_ModelCollection;//モデルリスト

	ID3D11ShaderResourceView** m_StarsSRV;

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
		CockPit,
	};

	void Init();//初期化
	void UnInit();

	void Draw(FileList File_);
};