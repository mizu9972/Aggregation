#pragma once

#include <Windows.h>
#include "definer.h"
#include "FbxModel.h"

//ゲーム進行のメインを司るシングルトンクラス
class CGameMain{
private:
	CGameMain() {};
	~CGameMain() = default;

public:

	CFbxModel *testModel;

	CGameMain(const CGameMain&) = delete;
	CGameMain(CGameMain&&) = delete;
	CGameMain operator = (const CGameMain&) = delete;
	CGameMain operator = (CGameMain&&) = delete;

	static CGameMain* GetInstance() {
		static CGameMain instance;
		return &instance;
	}

	bool Init(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen);//初期化

	void Main();//メインループ部分 <-[Input Update Render]

	void Input();//入力情報更新
	void Update();//更新
	void Render();//描画

	void Exit();//終了処理
};