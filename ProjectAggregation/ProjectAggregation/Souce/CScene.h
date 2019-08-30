#pragma once
#include "Observer.h"
//シーン管理クラスヘッダーファイル

//シーン基礎クラス
class SceneBase {
private:
public:

	virtual ~SceneBase() {};
	virtual void Init() = 0;//初期化
	virtual void Update() = 0;//更新
	virtual void Render() = 0;//描画
	virtual void UnInit() = 0;
	virtual SceneBase* NextScene() = 0;//次のシーンへの遷移
};

class Draw2D;
//タイトルシーンクラス
class TitleScene : public SceneBase ,public Observer{
private:
	bool isTitleEnd;
	Draw2D* m_TitleTex = nullptr;
public:
	TitleScene() { isTitleEnd = false; };
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void UnInit();
	virtual SceneBase* NextScene();
	virtual void OnNotify();//通知受け取り
};

//リザルトシーンクラス
class ResultScene : public SceneBase {
private:
	Draw2D* m_ResultTex = nullptr;
public:
	ResultScene() {};
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void UnInit();

	virtual SceneBase* NextScene();
};

class CModel;
//ゲームシーンクラス
class GameScene : public SceneBase , public Observer {
private:
	CModel* SkyDome = nullptr;
	CModel* StageModel = nullptr;
	bool isControlActive;
public:
	GameScene();
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void UnInit();

	virtual void OnNotify();//通知受け取り
	virtual SceneBase* NextScene();

};



//各シーンクラスのインスタンス所持クラス
class SceneInstance {
private:
public:
	static TitleScene Title;
	static GameScene Game;
	static ResultScene Result;
};