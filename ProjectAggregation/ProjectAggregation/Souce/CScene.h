#pragma once
//シーン管理クラスヘッダーファイル

//シーン基礎クラス
class SceneBase {
private:
public:

	virtual ~SceneBase() {};
	virtual void Init() = 0;//初期化
	virtual void Update() = 0;//更新
	virtual void Render() = 0;//描画

	virtual SceneBase* NextScene() = 0;//次のシーンへの遷移
};

//タイトルシーンクラス
class TitleScene : public SceneBase {
private:
public:
	TitleScene() {};
	virtual void Init();
	virtual void Update();
	virtual void Render();

	virtual SceneBase* NextScene();
};

//ゲームシーンクラス
class GameScene : public SceneBase {
private:
public:
	GameScene() {};
	virtual void Init();
	virtual void Update();
	virtual void Render();

	virtual SceneBase* NextScene();
};

//リザルトシーンクラス
class ResultScene : public SceneBase {
private:
public:
	ResultScene() {};
	virtual void Init();
	virtual void Update();
	virtual void Render();

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