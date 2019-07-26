#pragma once
#include "PmdLoader.h"

//外部から読み込んだデータを一括管理するクラス
class CFileLoader {
private:
	//CPmdLoader* m_Stage = nullptr;

	CFileLoader() {};
	~CFileLoader() = default;
public:
	CFileLoader(const CFileLoader&) = delete;
	CFileLoader(CFileLoader&&) = delete;
	CFileLoader operator = (const CFileLoader&) = delete;
	CFileLoader operator = (CFileLoader&&) = delete;

	static CFileLoader* GetInstance() {
		CFileLoader instance;
		return &instance;
	}

	void Init();//初期化
	void UnInit();
	void DrawStaticObject();//動かないオブジェクトの描画
};