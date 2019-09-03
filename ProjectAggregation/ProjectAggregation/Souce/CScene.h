#pragma once
#include <vector>
#include "Observer.h"
#include "Character.h"
#include "CParticle.h"
//�V�[���Ǘ��N���X�w�b�_�[�t�@�C��

//���L���[���h�s��
extern XMFLOAT4X4 CommonWorldMat;
//�V�[����b�N���X
class SceneBase {
private:
public:

	virtual ~SceneBase() {};
	virtual void Init() = 0;//������
	virtual void Update() = 0;//�X�V
	virtual void Render() = 0;//�`��
	virtual void UnInit() = 0;
	virtual void ObjectHitJudge() {};
	virtual SceneBase* NextScene() = 0;//���̃V�[���ւ̑J��
};

class Draw2D;
//�^�C�g���V�[���N���X
class TitleScene : public SceneBase ,public Observer{
private:
	bool isTitleEnd;
	Draw2D* m_TitleTex = nullptr;
	ParticleSystem* TitleParticle;
public:
	TitleScene() { isTitleEnd = false; };
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void UnInit();

	virtual SceneBase* NextScene();
	virtual void OnNotify();//�ʒm�󂯎��
};

//���U���g�V�[���N���X
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
//�Q�[���V�[���N���X
class GameScene : public SceneBase , public Observer {
private:
	CModel* StageModel = nullptr;
	CModel* CockPit    = nullptr;

	Draw2D* Site       = nullptr;
	Draw2D* ActiveSite = nullptr;

	Player* m_Player   = nullptr;

	std::vector<Character*> m_CharacterList;
	std::vector<ParticleSystem*> m_ParticleList;

	bool isControlActive;
public:
	GameScene();
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void UnInit();

	void ObjectHitJudge();//�G�l�~�[���Q���̓����蔻�菈��

	//�I�u�U�[�o�[�I�[�o�[���C�h
	//�ʒm�󂯎��
	virtual void OnNotify();
	virtual void OnNotify(Subject* subject_);
	virtual SceneBase* NextScene();

};



//�e�V�[���N���X�̃C���X�^���X�����N���X
class SceneInstance {
private:
public:
	static TitleScene Title;
	static GameScene Game;
	static ResultScene Result;
};