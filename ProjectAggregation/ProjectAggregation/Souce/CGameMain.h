#pragma once

#include <Windows.h>
#include <vector>
#include "definer.h"
#include "CEffectiveObject.h"

class SceneBase;

//ゲーム進行のメインを司るシングルトンクラス
class CGameMain{
private:
	CGameMain() {};
	~CGameMain() = default;

	//シーン
	SceneBase* m_NowScene;
	std::vector<CEffectiveObject*> m_EffectList;
public:

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

	void FeedInStart();
	void Exit();//終了処理
};