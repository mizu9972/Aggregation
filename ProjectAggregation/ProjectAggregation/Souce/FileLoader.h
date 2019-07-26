#pragma once
#include "PmdLoader.h"

//�O������ǂݍ��񂾃f�[�^���ꊇ�Ǘ�����N���X
class CFileLoader {
private:
	//CPmdLoader* m_Stage = nullptr;

	CFileLoader() {};
	~CFileLoader() = default;
public:
	CFileLoader(const CFileLoader&) = delete;
	CFileLoader(CFileLoader&&) = delete;
	CFileLoader operator = (const CFileLoader&) = delete;
	CFileLoader operator = (CFileLoader&&) = delete;

	static CFileLoader* GetInstance() {
		CFileLoader instance;
		return &instance;
	}

	void Init();//������
	void UnInit();
	void DrawStaticObject();//�����Ȃ��I�u�W�F�N�g�̕`��
};