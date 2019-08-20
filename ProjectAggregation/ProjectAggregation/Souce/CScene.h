#pragma once
//�V�[���Ǘ��N���X�w�b�_�[�t�@�C��

//�V�[����b�N���X
class SceneBase {
private:
public:

	virtual ~SceneBase() {};
	virtual void Init() = 0;//������
	virtual void Update() = 0;//�X�V
	virtual void Render() = 0;//�`��
	virtual void UnInit() = 0;
	virtual SceneBase* NextScene() = 0;//���̃V�[���ւ̑J��
};

//�^�C�g���V�[���N���X
class TitleScene : public SceneBase {
private:
public:
	TitleScene() {};
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void UnInit();
	virtual SceneBase* NextScene();
};

//���U���g�V�[���N���X
class ResultScene : public SceneBase {
private:
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
class GameScene : public SceneBase {
private:
	CModel* StageModel;
public:
	GameScene();
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void UnInit();

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