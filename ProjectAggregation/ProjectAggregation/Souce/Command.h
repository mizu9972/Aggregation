#pragma once
#include "CDirectInput.h"

//�L�[���̓r�b�g�V�t�g���X�g
//��{�ړ�
#define INPUT_UPTURN (1 << 0)
#define INPUT_DOWNTURN (1 << 1)
#define INPUT_RIGHTTURN (1 << 2)
#define INPUT_LEFTTURN (1 << 3)

#define INPUT_UP    (1 << 4)
#define INPUT_DOWN  (1 << 5)
#define INPUT_RIGHT (1 << 6)
#define INPUT_LEFT  (1 << 7)

//�U��
#define INPUT_SPACE (1 << 8)

//��]
#define INPUT_RSPIN (1 << 9)
#define INPUT_LSPIN (1 << 10)

//�ړ��֌W
#define INPUT_ACCEL (1 << 11)
#define INPUT_BRAKE (1 << 12)

class InputState {
private:
	//���͏����P�r�b�g���ۑ�����
	int m_InputState = 0;//�ۑ��p�ϐ�(32�r�b�g)

	//�L�[���͏���}������r�b�g�ʒu
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

		byte RSHIFT = 11;
		byte LSHIFT = 12;
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