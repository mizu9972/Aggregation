#pragma once

#include <Windows.h>
#include <vector>
#include "definer.h"
#include "CEffectiveObject.h"
#include "Observer.h"


class SceneBase;

//�Q�[���i�s�̃��C�����i��V���O���g���N���X
class CGameMain : public Observer ,public Subject{
private:
	CGameMain() {};
	~CGameMain() = default;

	//�V�[��
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

	bool Init(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen);//������

	void Main();//���C�����[�v���� <-[Input Update Render]

	void Input();//���͏��X�V
	void Update();//�X�V
	void Render();//�`��

	void FeedInStart(float MaxTime, XMFLOAT4 StartColor, XMFLOAT4 EndColor);
	void Exit();//�I������

	//�V�[���I�u�W�F�N�g�擾
	SceneBase* getNowScene() { return m_NowScene; };
	//�I�u�U�[�o�[�I�[�o�[���C�h
	virtual void OnNotify();

};