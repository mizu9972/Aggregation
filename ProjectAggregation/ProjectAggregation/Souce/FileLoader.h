#pragma once

#include "CModel.h"
//�O������ǂݍ��񂾃f�[�^���ꊇ�Ǘ�����N���X
class CFileLoader {
private:
	CModel* SkyDome;
	CModel* AirPlane;

	CFileLoader() {};
	~CFileLoader() = default;
public:
	CFileLoader(const CFileLoader&) = delete;
	CFileLoader(CFileLoader&&) = delete;
	CFileLoader operator = (const CFileLoader&) = delete;
	CFileLoader operator = (CFileLoader&&) = delete;

	static CFileLoader* GetInstance() {
		static CFileLoader instance;
		return &instance;
	}

	enum class FileList {
		AirPlane,
		SkyDome,
	};

	void Init();//������
	void UnInit();

	void Draw();//�����Ȃ��I�u�W�F�N�g�̕`��
	void Draw(FileList File_);
};