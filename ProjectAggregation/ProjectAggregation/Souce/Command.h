#pragma once
#include "CDirectInput.h"

//操作できるオブジェクトに継承させるクラス
class PlayerableObject {
public:
	//上下左右操作の仮想関数
	virtual void Up() = 0;
	virtual void Down() = 0;
	virtual void Right() = 0;
	virtual void Left() = 0;
};

//キー操作に対応して動作させる動きのインターフェースとなる基底クラス
class Command {
	//#修正不要
public:
	virtual ~Command() {};
	virtual void Action(PlayerableObject* Object) = 0;
};

//操作それぞれのクラス
/*操作を追加するには
PlayerableObjectに純粋仮想関数を追加して、
Commandを継承したその操作専用のクラスを作成し、
Actionメソッドをオーバーライドする
*/
class UpCommand :public Command {
	//上
public:
	virtual void Action(PlayerableObject* Object) {
		Object->Up();
	}
};

class DownCommand :public Command {
	//下
public:
	virtual void Action(PlayerableObject* Object) {
		Object->Down();
	}
};

class RightConmmand :public Command {
	//右
public:
	virtual void Action(PlayerableObject* Object) {
		Object->Right();
	}
};

class LeftCommand :public Command {
	//左
public:
	virtual void Action(PlayerableObject* Object) {
		Object->Left();
	}
};

//入力情報を返すシングルトンクラス
class InputHundler {
private:

	InputHundler() {
		//コマンドそれぞれ初期設定
		W_Command = new UpCommand;
		S_Command = new DownCommand;
		D_Command = new RightConmmand;
		A_Command = new LeftCommand;
	};
	~InputHundler() = default;

	//返すコマンド一覧
	Command* W_Command;
	Command* S_Command;
	Command* D_Command;
	Command* A_Command;
public:
	InputHundler(const InputHundler&) = delete;
	InputHundler(InputHundler&&) = delete;
	InputHundler operator = (const InputHundler&) = delete;
	InputHundler operator = (InputHundler&&) = delete;

	static InputHundler* getInstance() {
		static InputHundler instance;
		return &instance;
	}

	//入力キー情報取得
	Command* GetInputKey() {
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)) {
			return W_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S)) {
			return S_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D)) {
			return D_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {
			return A_Command;
		}
		return nullptr;
	}
};