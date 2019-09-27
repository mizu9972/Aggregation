#pragma once

#include "CModel.h"
#include "Shader.h"

//�O������ǂݍ��񂾃f�[�^���ꊇ�Ǘ�����N���X
class CFileLoader {
private:
	CModel* SkyDome;
	CModel* AirPlane;
	CModel* CockPit;
	
	struct ConstantBufferTime {
		double iTime;
		double Padding;
	};

	ID3D11Buffer* m_ConstantBufferTime;
	ID3D11ShaderResourceView** m_StarsSRV;
	ConstantBufferTime m_CurrentTime;

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
		CockPit,
	};

	void Init();//������
	void UnInit();

	void Draw();//�����Ȃ��I�u�W�F�N�g�̕`��
	void Draw(FileList File_);
};