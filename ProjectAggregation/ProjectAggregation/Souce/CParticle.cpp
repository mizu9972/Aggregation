//�p�[�e�B�N���V�X�e���v���O����

//�w�b�_�[
#include <random>
#include "CParticle.h"
#include "dx11mathutil.h"
#include "CCamera.h"

#define     FPS 60
#define		SCREEN_X		1200
#define		SCREEN_Y		600
#ifndef     ALIGN16
#define     ALIGN16 _declspec(align(16))
#endif
/*------------------------
�P�ʍs��ɂ���
--------------------------*/
void DX11MatrixIdentity_subDefiner(DirectX::XMFLOAT4X4& mat) {

	ALIGN16 XMMATRIX mtx;

	mtx = XMMatrixIdentity();

	XMStoreFloat4x4(&mat, mtx);
}

//���\�b�h
void ParticleSystem::Init(t_ParticleSystemState ParticleState_) {
	if (m_TexName == nullptr) {
		return;
	}
	SetParticleSystemState(ParticleState_);
	m_BillBoard.Init(0, 0, 0, m_ParticleState.m_Size, m_ParticleState.m_Size, XMFLOAT4(m_ParticleState.m_Color[0], m_ParticleState.m_Color[1], m_ParticleState.m_Color[2], ParticleState_.m_Color[3]));

	Start();
}
void ParticleSystem::Init(t_ParticleSystemState ParticleState_, const char* filename) {
	// �t�@�C������ۑ�
	m_TexName = filename;
	XMFLOAT4 col = { ParticleState_.m_Color[0],ParticleState_.m_Color[1],ParticleState_.m_Color[2],ParticleState_.m_Color[3] };

	SetParticleSystemState(ParticleState_);
	m_BillBoard.Init(0, 0, 0,m_ParticleState.m_Size, m_ParticleState.m_Size, XMFLOAT4(m_ParticleState.m_Color[0], m_ParticleState.m_Color[1], m_ParticleState.m_Color[2], m_ParticleState.m_Color[3]));

	float u[4] = { 0.0f, 0.0f, 1.0f, 1.0f};
	float v[4] = { 1.0f, 0.0f, 1.0f, 0.0f };

	m_BillBoard.SetUV(u, v);

	m_BillBoard.LoadTexTure(m_TexName);

}

void ParticleSystem::ZeroInit() {
	if (m_ParticleStateZero != NULL) {

		SetParticleSystemState(*m_ParticleStateZero);

		Start();
	}

}


void ParticleSystem::Start() {
	//�p�[�e�B�N�������J�n
	//�p�[�e�B�N������
	if (Particles != NULL) {
		delete[] Particles;
	}
	Particles = new m_Particles[m_ParticleState.m_ParticleNum];
	isSystemActive = true;
	ParticlesDeathCount = 0;

	m_SystemLifeTime = m_ParticleState.m_DuaringTime;
	//�p�[�e�B�N���ݒ�-------------------------------------------------------------------------
	srand((int)NowTime);
	
	XMFLOAT3 SetRandAngle;
	XMFLOAT4 qt;//�N�H�[�^�j�I��
	XMFLOAT4 axisX, axisY, axisZ;//�x�N�g���ۑ�
	XMFLOAT4 qtx, qty, qtz;//�N�H�[�^�j�I��
	XMFLOAT4 tempqt1, tempqt2, tempqt3;//�ꎞ�ۑ��N�H�[�^�j�I��
	m_Particles* p_PickParticle;//���삷��p�[�e�B�N���̃|�C���^�ۑ��p

	//�p�[�e�B�N���̏����ݒ肷��
	for (int ParticlesNum = 0; ParticlesNum < m_ParticleState.m_ParticleNum; ParticlesNum++) {

		p_PickParticle = &Particles[ParticlesNum];

		DX11MatrixIdentity_subDefiner(p_PickParticle->Matrix);
		//�����_���Ȋp�x�����ꂼ��ݒ�
		SetRandAngle.x = (float)(rand() % m_ParticleState.m_AngleRangeX);
		SetRandAngle.y = (float)(rand() % m_ParticleState.m_AngleRangeY);
		SetRandAngle.z = (float)(rand() % m_ParticleState.m_AngleRangeZ);

		//�p�x�����o��-------------------------------
		axisX.x = p_PickParticle->Matrix._11;
		axisX.y = p_PickParticle->Matrix._12;
		axisX.z = p_PickParticle->Matrix._13;

		axisY.x = p_PickParticle->Matrix._21;
		axisY.y = p_PickParticle->Matrix._22;
		axisY.z = p_PickParticle->Matrix._23;

		axisZ.x = p_PickParticle->Matrix._31;
		axisZ.y = p_PickParticle->Matrix._32;
		axisZ.z = p_PickParticle->Matrix._33;
		//-------------------------------------------

		//�s�񂩂�N�H�[�^�j�I���𐶐�
		DX11GetQtfromMatrix(p_PickParticle->Matrix, qt);


		//�w�莲��]�̃N�H�[�^�j�I���𐶐�
		DX11QtRotationAxis(qtx, axisX, SetRandAngle.x + m_ParticleState.m_AngleX);
		DX11QtRotationAxis(qty, axisY, SetRandAngle.y + m_ParticleState.m_AngleY);
		DX11QtRotationAxis(qtz, axisZ, SetRandAngle.z + m_ParticleState.m_AngleZ);

		//�N�H�[�^�j�I���𐳋K��

		//�N�H�[�^�j�I��������

		DX11QtMul(tempqt1, qt, qtx);

		DX11QtMul(tempqt2, qty, qtz);

		DX11QtMul(tempqt3, tempqt1, tempqt2);

		//�N�H�[�^�j�I�����m�[�}���C�Y
		DX11QtNormalize(tempqt3, tempqt3);

		DX11MtxFromQt(p_PickParticle->Matrix, tempqt3);

		p_PickParticle->Matrix._41 = m_ParticleState.m_PositionX;
		p_PickParticle->Matrix._42 = m_ParticleState.m_PositionY;
		p_PickParticle->Matrix._43 = m_ParticleState.m_PositionZ;

		//�����܂ł̑ҋ@���Ԑݒ�
		//�p�[�e�B�N���̔������Ԃ��p�[�e�B�N�����œ������A���ꂼ�ꊄ�蓖�Ă�
		p_PickParticle->DelayTime = m_ParticleState.m_DuaringTime / m_ParticleState.m_ParticleNum * ParticlesNum;
		//���̑��ݒ�
		p_PickParticle->LifeTime = m_ParticleState.m_MaxLifeTime;

		//�ҋ@����
		p_PickParticle->ZAngle = rand() % 360;
		p_PickParticle->isAlive = false;
		p_PickParticle->isWaiting = true;
	}
	//--------------------------------------------------------------------------------------
}

void ParticleSystem::Update() {
	//�X�V����
	float ParticleSize;//�T�C�Y�ω��p
	m_Particles* p_PickParticle;//���삷��p�[�e�B�N���̃|�C���^�ۑ��p


	if (isSystemActive == false) {
		return;
	}

	float Speed_ = m_ParticleState.m_Speed;
	NowTime = 1.0f / FPS;
	for (int ParticlesNum = 0; ParticlesNum < m_ParticleState.m_ParticleNum; ParticlesNum++) {

		p_PickParticle = &Particles[ParticlesNum];

		//�ҋ@���Ȃ�ҋ@���Ԍ���
		if (p_PickParticle->isWaiting == true) {
			p_PickParticle->DelayTime -= NowTime;

			//�ҋ@���Ԃ�0�ȉ��Ȃ犈����
			if (p_PickParticle->DelayTime <= 0) {
				p_PickParticle->isWaiting = false;
				p_PickParticle->isAlive = true;
			}
		}

		if (p_PickParticle->isAlive == false) {
			continue;
		}
		//�X�V����-------------------------------------------------------------------
		if (p_PickParticle->Matrix._43 != 0.0f) {
			ParticleSize = m_ParticleState.m_Size / p_PickParticle->Matrix._43;
		}
		else {
			ParticleSize = 1.0f;
		}
		
		m_ParticleState.m_Size += ParticleSize;

		//���x���ړ�������
		p_PickParticle->Matrix._41 += p_PickParticle->Matrix._31 * Speed_ * NowTime;
		p_PickParticle->Matrix._42 += p_PickParticle->Matrix._32 * Speed_ * NowTime;
		p_PickParticle->Matrix._43 += p_PickParticle->Matrix._33 * Speed_ * NowTime;

		//�������Ԍ���
		p_PickParticle->LifeTime -= NowTime;

		p_PickParticle->ZAngle += m_ParticleState.m_RotateSpeed;
		p_PickParticle->ZAngle = p_PickParticle->ZAngle % 360;
		//--------------------------------------------------------------------------

		//���S����
		if (p_PickParticle->LifeTime <= 0) {
			p_PickParticle->isAlive = false;
			ParticlesDeathCount += 1;
		}
	}
	m_SystemLifeTime -= NowTime;

	
	//�p�[�e�B�N���I������
	if (m_SystemLifeTime <= 0) {
		isSystemActive = false;
		if (m_ParticleState.isLooping == true) {//���[�v
			Start();
		}
	}
}

void ParticleSystem::Draw() {
	if (isSystemActive == false) {
		return;
	}

	for (int ParticlesNum = 0; ParticlesNum < m_ParticleState.m_ParticleNum; ParticlesNum++) {
		if (Particles[ParticlesNum].isAlive == false) {
			continue;
		}

		// �`��

		m_BillBoard.SetPosition(Particles[ParticlesNum].Matrix._41, Particles[ParticlesNum].Matrix._42, Particles[ParticlesNum].Matrix._43);
		if (m_ParticleState.isAlphaBlend) {
			m_BillBoard.DrawRotateBillBoardAlpha(CCamera::GetInstance()->GetCameraMatrix(), (float)Particles[ParticlesNum].ZAngle);
		}
		else {
			m_BillBoard.DrawRotateBillBoard(CCamera::GetInstance()->GetCameraMatrix(), (float)Particles[ParticlesNum].ZAngle);
		}
	}

}

void ParticleSystem::UnInit() {
	if (Particles != NULL) {
		delete[] Particles;
	}
}

bool ParticleSystem::FInState(const char* FileName_) {
	//�p�[�e�B�N���V�X�e���̏����t�@�C������ǂݍ���

	t_ParticleSystemState GetState;

	//�t�@�C���ǂݍ���
	FILE *Fp;
	errno_t ErrorCheck;
	try {
		ErrorCheck = fopen_s(&Fp, FileName_, "rb");//

		if (ErrorCheck != 0) {
			throw 1;
		}
		fread(&GetState, sizeof(t_ParticleSystemState), 1, Fp);//�f�[�^�ǂݍ���
		
		SetParticleSystemState(GetState);//���Z�b�g

		fclose(Fp);
	}
	catch (int i) {//�t�@�C���ǂݍ��ݎ��s������
		return i;
	};

	Init(m_ParticleState, "assets/ParticleTexture/DefaultParticle.png");
	return true;
}

bool ParticleSystem::FInState(const char* DataName_ , const char* TexName_) {
	//�p�[�e�B�N���V�X�e���̏����t�@�C������ǂݍ���

	t_ParticleSystemState GetState;

	//�t�@�C���ǂݍ���
	FILE *Fp;
	errno_t ErrorCheck;
	try {
		ErrorCheck = fopen_s(&Fp, DataName_, "rb");//

		if (ErrorCheck != 0) {
			throw 1;
		}
		fread(&GetState, sizeof(t_ParticleSystemState), 1, Fp);//�f�[�^�ǂݍ���

		SetParticleSystemState(GetState);//���Z�b�g

		fclose(Fp);
	}
	catch (int i) {//�t�@�C���ǂݍ��ݎ��s������
		return i;
	};

	Init(m_ParticleState, TexName_);
	return true;
}

bool ParticleSystem::FInTex(const char* FileName_) {
	//�e�N�X�`���ǂݍ���
	std::string Texname = ".\\InPutData/";
	Texname += FileName_;//�t�@�C���̈ʒu���w��

	Init(m_ParticleState, Texname.c_str());//�w�肵���t�@�C���𗘗p���ď�����

	return 0;
}

void ParticleSystem::FOutState() {
	//�p�[�e�B�N���V�X�e���̏����t�@�C���֏����o��

	//�J�����g�f�B���N�g���𑀍삷��
	TCHAR crDir[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH + 1, crDir);//����O�̃f�B���N�g�����擾
	SetCurrentDirectory(".\\OutPutData");//�����o����̃t�H���_�փJ�����g�f�B���N�g����ύX

	//�t�@�C�������o��
	FILE *Fp;

	fopen_s(&Fp, "ParticleState.txt", "wb");

	fwrite(&m_ParticleState, sizeof(t_ParticleSystemState), 1, Fp);

	fclose(Fp);

	SetCurrentDirectory(crDir);//�J�����g�f�B���N�g�������ɖ߂�
}

//�A�N�Z�X
void ParticleSystem::SetParticleSystemState(t_ParticleSystemState SetParticleSystemState_) {
	memcpy(&m_ParticleState, &SetParticleSystemState_, sizeof(t_ParticleSystemState));
}

void ParticleSystem::SetPos(float x_, float y_, float z_) {
	m_ParticleState.m_PositionX = x_;
	m_ParticleState.m_PositionY = y_;
	m_ParticleState.m_PositionZ = z_;
}

void ParticleSystem::RefParticle() {

}