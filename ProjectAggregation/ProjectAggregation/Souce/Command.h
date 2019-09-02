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

//���͏���Ԃ��V���O���g���N���X
class InputHundler {
private:

	InputHundler() {
		//�R�}���h���ꂼ�ꏉ���ݒ�
		W_Command = new UpCommand;
		S_Command = new DownCommand;
		D_Command = new RightConmmand;
		A_Command = new LeftCommand;
	};
	~InputHundler() = default;

	//�Ԃ��R�}���h�ꗗ
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
		return nullptr;
	}
};