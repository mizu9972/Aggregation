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
	virtual void R_Turn() = 0;
	virtual void L_Turn() = 0;
	virtual void Act() = 0;

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

class R_TurnCommand : public Command {
	//右回転
public:
	virtual void Action(PlayerableObject* Object) {
		Object->R_Turn();
	}
};

class L_TurnCommand : public Command {
	//左回転
public:
	virtual void Action(PlayerableObject* Object) {
		Object->L_Turn();
	}
};
class ActCommand : public Command {
	//行動キー
public:
	virtual void Action(PlayerableObject* Object) {
		Object->Act();
	}
};

//入力情報を返すシングルトンクラス
class InputHundler {
private:

	InputHundler() {
		//コマンドそれぞれ初期設定
		//オーバーライドしなおすとキー設定の変更可能
		W_Command = new UpCommand;
		S_Command = new DownCommand;
		D_Command = new RightConmmand;
		A_Command = new LeftCommand;
		Q_Command = new L_TurnCommand;
		E_Command = new R_TurnCommand;
		SPACE_Command = new ActCommand;
	};
	~InputHundler() = default;

	//返すコマンド一覧
	Command* W_Command;
	Command* S_Command;
	Command* D_Command;
	Command* A_Command;
	Command* Q_Command;
	Command* E_Command;
	Command* SPACE_Command;
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
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_Q)) {
			return Q_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_E)) {
			return E_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
			return SPACE_Command;
		}
		return nullptr;
	}

	//同時押しされているキー取得
	Command* GetInputSecondKey(Command* FirstCommand) {
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W) && FirstCommand != W_Command) {
			return W_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S) && FirstCommand != S_Command) {
			return S_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D) && FirstCommand != D_Command) {
			return D_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A) && FirstCommand != A_Command) {
			return A_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_Q) && FirstCommand != Q_Command) {
			return Q_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_E) && FirstCommand != E_Command) {
			return E_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_SPACE) && FirstCommand != SPACE_Command) {
			return SPACE_Command;
		}
		return nullptr;
	}
};