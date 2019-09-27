#pragma once
#include	<directxmath.h>
#include    "definer.h"

using namespace DirectX;
//カメラシングルトンクラス
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

		SetProjection(nearclip, farclip, fov, width, height);
		SetCamera(eye, lookat, up);
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

	/*void SetTrandform(DX11SetTransform::TYPE::VIEW, mat) {

	}*/
	void CreateCameraMatrix() {
		ALIGN16 XMVECTOR Eye = XMVectorSet(m_Eye.x, m_Eye.y, m_Eye.z, 0.0f);
		ALIGN16 XMVECTOR At = XMVectorSet(m_Lookat.x, m_Lookat.y, m_Lookat.z, 0.0f);
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
		//水平方向の回転角度を計算し返す

		//注視点への向きベクトルを求める
		XMFLOAT3 Direct;
		Direct.x = LookAt->x - Eye->x;
		Direct.y = LookAt->y - Eye->y;
		Direct.z = LookAt->z - Eye->z;

		//XZ平面での角度を求める
		float deg = atan2f(-Direct.z, Direct.x);
		deg = XMConvertToDegrees(deg);//ラジアンから度数法へ

		deg += 90.0f;//角度を調整

		//-180~180の間におさめる
		if (deg > 180.0f) {
			deg -= 360.0f;
		}
		else if (deg < -180.0f) {
			deg += 360.0f;
		}

		return deg;
	}

	float GetVerticalAngle(XMFLOAT3* Eye, XMFLOAT3* LookAt) {
		//垂直方向の回転角度を計算し返す

		//注視点への向きベクトルを求める
		XMFLOAT3 Direct;
		Direct.x = LookAt->x - Eye->x;
		Direct.y = LookAt->y - Eye->y;
		Direct.z = LookAt->z - Eye->z;

		float fFront;
		{
			//カメラの前方方向の値
			XMFLOAT3 vFront;
			vFront.x = Direct.x;
			vFront.y = 0;
			vFront.z = Direct.z;
			fFront = sqrtf(vFront.x * vFront.x + vFront.z * vFront.z);
		}

		float deg = atan2f(Direct.y, fFront);

		//-90~90の間におさめる
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

private:
	XMFLOAT4X4		m_Projection;

	XMFLOAT4X4		m_Camera;

	XMFLOAT3		m_Eye;				// カメラ位置
	XMFLOAT3		m_Lookat;			// 注視点
	XMFLOAT3		m_Up;				// 上向きベクトル

	float			m_near;
	float			m_Aspect;
	float			m_Fov;
	float			m_far;
};