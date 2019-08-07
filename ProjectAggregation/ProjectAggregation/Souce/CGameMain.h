#pragma once

#include <Windows.h>
#include "definer.h"
#include "FbxModel.h"

//�Q�[���i�s�̃��C�����i��V���O���g���N���X
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

	bool Init(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen);//������

	void Main();//���C�����[�v���� <-[Input Update Render]

	void Input();//���͏��X�V
	void Update();//�X�V
	void Render();//�`��

	void Exit();//�I������
};