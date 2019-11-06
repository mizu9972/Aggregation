#pragma once
#include "CDirectInput.h"

//キー入力ビットシフトリスト
//基本移動
#define INPUT_W (1 << 0)
#define INPUT_S (1 << 1)
#define INPUT_D (1 << 2)
#define INPUT_A (1 << 3)

#define INPUT_UP    (1 << 4)
#define INPUT_DOWN  (1 << 5)
#define INPUT_RIGHT (1 << 6)
#define INPUT_LEFT  (1 << 7)

//攻撃
#define INPUT_SPACE (1 << 8)

//回転
#define INPUT_E (1 << 9)
#define INPUT_Q (1 << 10)

class InputState {
private:
	//入力情報を１ビットずつ保存する
	int m_InputState = 0;//保存用変数(32ビット)

	//キー入力情報を挿入するビット位置
	struct KeyStateBitPoint {
		byte Wkey = 0;
		byte Skey = 1;
		byte Dkey = 2;
		byte Akey = 3;

		byte Upkey    = 4;
		byte Downkey  = 5;
		byte Rightkey = 6;
		byte Leftkey  = 7;

		byte Spacekey = 8;

		byte Ekey = 9;
		byte Qkey = 10;
	};
	KeyStateBitPoint m_keyStateBitPoint;

	InputState() {};
	~InputState() = default;

public:
	InputState(const InputState&) = delete;
	InputState(InputState&&) = delete;
	InputState operator = (const InputState&) = delete;
	InputState operator = (InputState&&) = delete;

	static InputState* GetInstance() {
		static InputState instance;
		return &instance;
	}

	void Update();
	bool getInput(int key);
};