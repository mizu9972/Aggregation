#pragma once
#include "CDirectInput.h"

//����ł���I�u�W�F�N�g�Ɍp��������N���X
class PlayerableObject {
public:
	//�㉺���E����̉��z�֐�
	virtual void Up() = 0;
	virtual void Down() = 0;
	virtual void Right() = 0;
	virtual void Left() = 0;
	virtual void R_Turn() = 0;
	virtual void L_Turn() = 0;
	virtual void Act() = 0;

	virtual void SubUp() = 0;
	virtual void SubDown() = 0;
	virtual void SubRight() = 0;
	virtual void SubLeft() = 0;

};

//�L�[����ɑΉ����ē��삳���铮���̃C���^�[�t�F�[�X�ƂȂ���N���X
class Command {
	//#�C���s�v
public:
	virtual ~Command() {};
	virtual void Action(PlayerableObject* Object) = 0;
};

//���삻�ꂼ��̃N���X
/*�����ǉ�����ɂ�
PlayerableObject�ɏ������z�֐���ǉ����āA
Command���p���������̑����p�̃N���X���쐬���A
Action���\�b�h���I�[�o�[���C�h����
*/
class UpCommand :public Command {
	//��
public:
	virtual void Action(PlayerableObject* Object) {
		Object->Up();
	}
};

class DownCommand :public Command {
	//��
public:
	virtual void Action(PlayerableObject* Object) {
		Object->Down();
	}
};

class RightConmmand :public Command {
	//�E
public:
	virtual void Action(PlayerableObject* Object) {
		Object->Right();
	}
};

class LeftCommand :public Command {
	//��
public:
	virtual void Action(PlayerableObject* Object) {
		Object->Left();
	}
};

class R_TurnCommand : public Command {
	//�E��]
public:
	virtual void Action(PlayerableObject* Object) {
		Object->R_Turn();
	}
};

class L_TurnCommand : public Command {
	//����]
public:
	virtual void Action(PlayerableObject* Object) {
		Object->L_Turn();
	}
};
class ActCommand : public Command {
	//�s���L�[
public:
	virtual void Action(PlayerableObject* Object) {
		Object->Act();
	}
};

class SubUpCommand :public Command {
	//�T�u��
public:
	virtual void Action(PlayerableObject* Object) {
		Object->SubUp();
	}
};

class SubDownCommand :public Command {
	//�T�u��
public:
	virtual void Action(PlayerableObject* Object) {
		Object->SubDown();
	}
};

class SubRightCommand :public Command {
	//�T�u�E
public:
	virtual void Action(PlayerableObject* Object) {
		Object->SubRight();
	}
};

class SubLeftCommand :public Command {
	//�T�u��
public:
	virtual void Action(PlayerableObject* Object) {
		Object->SubLeft();
	}
};
//���͏���Ԃ��V���O���g���N���X
class InputHundler {
private:

	InputHundler() {
		//�R�}���h���ꂼ�ꏉ���ݒ�
		//�I�[�o�[���C�h���Ȃ����ƃL�[�ݒ�̕ύX�\
		W_Command = new UpCommand;
		S_Command = new DownCommand;
		D_Command = new RightConmmand;
		A_Command = new LeftCommand;
		Q_Command = new L_TurnCommand;
		E_Command = new R_TurnCommand;
		SPACE_Command = new ActCommand;

		Up_Command = new SubUpCommand;
		Down_Command = new SubDownCommand;
		Right_Command = new SubRightCommand;
		Left_Command = new SubLeftCommand;
	};
	~InputHundler() = default;

	//�Ԃ��R�}���h�ꗗ
	Command* W_Command;
	Command* S_Command;
	Command* D_Command;
	Command* A_Command;
	Command* Q_Command;
	Command* E_Command;
	Command* SPACE_Command;
	Command* Up_Command;
	Command* Down_Command;
	Command* Right_Command;
	Command* Left_Command;
public:
	InputHundler(const InputHundler&) = delete;
	InputHundler(InputHundler&&) = delete;
	InputHundler operator = (const InputHundler&) = delete;
	InputHundler operator = (InputHundler&&) = delete;

	static InputHundler* getInstance() {
		static InputHundler instance;
		return &instance;
	}

	//���̓L�[���擾
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
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP)) {
			return Up_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN)) {
			return Down_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT)) {
			return Right_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT)) {
			return Left_Command;
		}
		return nullptr;
	}

	//������������Ă���L�[�擾
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
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) && FirstCommand != Up_Command) {
			return Up_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) && FirstCommand != Down_Command) {
			return Down_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) && FirstCommand != Right_Command) {
			return Right_Command;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) && FirstCommand != Left_Command) {
			return Left_Command;
		}
		return nullptr;
	}
};