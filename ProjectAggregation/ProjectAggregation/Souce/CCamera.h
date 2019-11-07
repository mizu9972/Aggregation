#pragma once
#include	<directxmath.h>
#include	<Windows.h>
#include	<random>
#include	<time.h>
#include    "definer.h"

//�w��͈͓̔��Ń����_����int�^���l��Ԃ�
#define RandomRange(minNum,maxNum) (rand() % (maxNum - minNum) + minNum)

using namespace DirectX;
//�J�����V���O���g���N���X
class CCamera {
private:

	CCamera() {};
	~CCamera() = default;
public:
	CCamera(const CCamera&) = delete;
	CCamera(CCamera&&) = delete;
	CCamera operator = (const CCamera&) = delete;
	CCamera operator = (CCamera&&) = delete;

	static CCamera* GetInstance() {
		static CCamera instance;
		return &instance;
	}

	void Init(float nearclip, float farclip, float fov,
		float width, float height,
		XMFLOAT3 eye, XMFLOAT3 lookat, XMFLOAT3 up) {
		srand((unsigned)time(NULL));

		SetProjection(nearclip, farclip, fov, width, height);
		SetCamera(eye, lookat, up);
	}

	void Update() {

		if (m_Shaking) {
			ShakeCamera();
		}
	}

	void SetNear(float nearclip) {
		m_near = nearclip;
	}

	void SetFar(float farclip) {
		m_far = farclip;
	}

	void SetFov(float fov) {
		m_Fov = fov;
	}

	void SetAspect(float width, float height) {
		m_Aspect = width / height;
	}

	void SetProjection(float nearclip, float farclip, float fov, float width, float height) {
		SetNear(nearclip);
		SetFar(farclip);
		SetFov(fov);
		SetAspect(width, height);
		CreateProjectionMatrix();
	}

	void SetCamera(const XMFLOAT3& eye, const XMFLOAT3& lookat, const XMFLOAT3& up) {

		SetEye(eye);
		SetLookat(lookat);
		SetUp(up);
		CreateCameraMatrix();
	}

	void SetEye(const XMFLOAT3& eye) {
		m_Eye = eye;
	}

	void SetLookat(const XMFLOAT3& lookat) {
		m_Lookat = lookat;
	}

	void SetUp(const XMFLOAT3& up) {
		m_Up = up;
	}

	void StartShake(float duaring,int minNum = 5,int maxNum = 10) {
		if (m_Shaking == true) {
			return;
		}
		m_Shaking = true;
		m_ShakDuaring = duaring;
		m_ShakeTime = 0;
		m_ShakeOldTime = 0;
		m_SaveEye = m_Eye;

		//�h��̉񐔂ɗ��p���郉���_�����l�ݒ�
		m_ShakeRandomNum.x = RandomRange(minNum, maxNum);
		m_ShakeRandomNum.y = RandomRange(minNum, maxNum);
		m_ShakeRandomNum.z = RandomRange(minNum, maxNum);
	}

	/*void SetTrandform(DX11SetTransform::TYPE::VIEW, mat) {

	}*/
	void CreateCameraMatrix() {
		ALIGN16 XMVECTOR Eye = XMVectorSet(m_Eye.x + m_SubEye.x, m_Eye.y + m_SubEye.y, m_Eye.z + m_SubEye.z, 0.0f);
		ALIGN16 XMVECTOR At = XMVectorSet(m_Lookat.x + m_SubLookat.x, m_Lookat.y + m_SubLookat.y, m_Lookat.z + m_SubLookat.z, 0.0f);
		ALIGN16 XMVECTOR Up = XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0.0f);

		ALIGN16 XMMATRIX camera;
		camera = XMMatrixLookAtLH(Eye, At, Up);

		XMStoreFloat4x4(&m_Camera, camera);
	}

	void CreateProjectionMatrix() {

		ALIGN16 XMMATRIX projection;

		projection = XMMatrixPerspectiveFovLH(m_Fov, m_Aspect, m_near, m_far);

		XMStoreFloat4x4(&m_Projection, projection);

	}

	float GetHorizonAngle(XMFLOAT3* Eye,XMFLOAT3* LookAt) {
		//���������̉�]�p�x���v�Z���Ԃ�

		//�����_�ւ̌����x�N�g�������߂�
		XMFLOAT3 Direct;
		Direct.x = LookAt->x - Eye->x;
		Direct.y = LookAt->y - Eye->y;
		Direct.z = LookAt->z - Eye->z;

		//XZ���ʂł̊p�x�����߂�
		float deg = atan2f(-Direct.z, Direct.x);
		deg = XMConvertToDegrees(deg);//���W�A������x���@��

		deg += 90.0f;//�p�x�𒲐�

		//-180~180�̊Ԃɂ����߂�
		if (deg > 180.0f) {
			deg -= 360.0f;
		}
		else if (deg < -180.0f) {
			deg += 360.0f;
		}

		return deg;
	}

	float GetVerticalAngle(XMFLOAT3* Eye, XMFLOAT3* LookAt) {
		//���������̉�]�p�x���v�Z���Ԃ�

		//�����_�ւ̌����x�N�g�������߂�
		XMFLOAT3 Direct;
		Direct.x = LookAt->x - Eye->x;
		Direct.y = LookAt->y - Eye->y;
		Direct.z = LookAt->z - Eye->z;

		float fFront;
		{
			//�J�����̑O�������̒l
			XMFLOAT3 vFront;
			vFront.x = Direct.x;
			vFront.y = 0;
			vFront.z = Direct.z;
			fFront = sqrtf(vFront.x * vFront.x + vFront.z * vFront.z);
		}

		float deg = atan2f(Direct.y, fFront);

		//-90~90�̊Ԃɂ����߂�
		if (deg > 90.0f) {
			deg = 180.0f - deg;
		}
		else if(deg < -90.0f){
			deg = -180.0f - deg;
		}

		return deg;
	}



	const XMFLOAT4X4& GetCameraMatrix() {
		return m_Camera;
	}

	const XMFLOAT4X4& GetProjectionMatrix() {
		return m_Projection;
	}

	float GetFov() const {
		return m_Fov;
	}

	const XMFLOAT3& GetEye() const {
		return m_Eye;
	}

	const XMFLOAT3& GetLookat() const {
		return m_Lookat;
	}

	const XMFLOAT3& GetUp() const {
		return m_Up;
	}

	const bool GetisShaking()const {
		return m_Shaking;
	}

private:
	XMFLOAT4X4		m_Projection;

	XMFLOAT4X4		m_Camera;

	XMFLOAT3		m_Eye;				// �J�����ʒu
	XMFLOAT3		m_SubEye;
	XMFLOAT3		m_Lookat;			// �����_
	XMFLOAT3		m_SubLookat;
	XMFLOAT3		m_Up;				// ������x�N�g��

	float			m_near;
	float			m_Aspect;
	float			m_Fov;
	float			m_far;
	bool			m_Shaking = false;
	float			m_ShakDuaring;
	float			m_ShakeTime = 0;
	float			m_ShakeOldTime = 0;
	XMFLOAT3		m_SaveEye;
	XMINT3		m_ShakeRandomNum;

	//�J������h�炷
	void ShakeCamera() {
		float MaxTime = m_ShakDuaring * 1000.0f;
		float NowTime = static_cast<float>(timeGetTime());
		if (m_ShakeOldTime == 0) {//�J�n������
			m_ShakeOldTime = NowTime;
		}

		//�J���������ۂɗh�炷����
		//�����g�𗘗p���ėh���悤�ȓ�����������
		auto ShakeFunc = [=](int x) { return (float)(sinf((m_ShakeTime / (MaxTime / (2 * XM_PI))) * x) * 0.1); };

		//�o�ߎ��Ԍv�Z
		m_ShakeTime += NowTime - m_ShakeOldTime;
		m_ShakeOldTime = NowTime;

		//�ʒu���f
		m_SubEye.x = ShakeFunc(m_ShakeRandomNum.x);
		m_SubEye.y = ShakeFunc(m_ShakeRandomNum.y);
		m_SubLookat.x = ShakeFunc(m_ShakeRandomNum.x);
		m_SubLookat.y = ShakeFunc(m_ShakeRandomNum.y);

		CreateCameraMatrix();

		//�I��������
		if (m_ShakeTime >= MaxTime) {
			m_ShakeTime = 0;
			m_ShakeOldTime = 0;

			m_SubEye = XMFLOAT3(0, 0, 0);
			m_SubLookat = XMFLOAT3(0, 0, 0);
			m_Shaking = false;
		}
	}
};