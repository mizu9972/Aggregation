#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <memory>
#include <string>
#include <vector>
#include "WindowSetting.h"

using namespace DirectX;

// ���_�\���̒�`
struct SimpleVertex
{
	XMFLOAT3 Pos;						// XNA MATH���C�u�����Ŏg�p�ł���悤�ɒ�`���ꂽ�\���́@float*3 
	XMFLOAT4 Color;						// �J���[�l
	XMFLOAT3 Normal;					// �@���x�N�g��
	XMFLOAT2 Tex;						// �e�N�X�`�����W
	XMFLOAT4 BoneIndex;					// �{�[���s��ւ̃C���f�b�N�X�S��
	XMFLOAT4 BoneWeighs;				// �{�[���E�F�C�g�S��
};

// �萔�o�b�t�@��`
struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT3 mLightDirection;
	XMFLOAT3 mCameraPos;
};

struct ConstantBufferBoneMatrix			// �{�[���s��i�[�p�̃R���X�^���g�o�b�t�@
{
	XMMATRIX mBoneMatrix[400];
};

struct ConstantBufferMaterial
{
	XMFLOAT4	diffuse;				// �f�B�t���[�Y
	XMFLOAT4	specular;				// �X�y�L����
	XMFLOAT4	ambient;				// �A���r�G���g
	XMFLOAT4	power;					// �p���[�l
};


struct sVertex_Point
{
	XMFLOAT3 v;
};


struct Matrix4x4
{
	union
	{
		float   m[16];				// �P�����z��
		float	m2[4][4];			// �Q�����z��
	};
	void transpose()
	{
		m2[0][1] = m2[1][0];
		m2[0][2] = m2[2][0];
		m2[0][3] = m2[3][0];
		m2[1][2] = m2[2][1];
		m2[1][3] = m2[3][1];
		m2[2][3] = m2[3][2];
	}
};

struct Vector3
{
	float x, y, z;
	Vector3(float p1, float p2, float p3)
	{
		x = p1; y = p2; z = p3;
	}
	Vector3()
	{
		x = y = z = 0.0f;
	}
};

struct Vector2
{
	float	x, y;
	Vector2(float p1, float p2)
	{
		x = p1; y = p2;
	}
	Vector2() {
		x = y = 0.0f;
	}
};

struct Vector4
{
	float v[4];
	Vector4(float x, float y, float z, float w)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = w;
	}
	Vector4()
	{
		v[0] = v[1] = v[2] = v[3] = 0.0f;
	}
};
// �{�[�����
struct ModelBoneWeight
{
	unsigned int boneIndex[4];				// �{�[���C���f�b�N�X�ԍ�
	Vector4		boneWeight;					// �{�[���E�G�C�g�l
};

struct sConstantBuffer_Point
{
	XMMATRIX mWVP;
};

struct ModelVertex
{
	Vector3 position;						// ���W
	Vector3 normal;							// �@�����
	Vector2 uv0;							// �e�N�X�`�����W
	unsigned int	boneIndex[4];			// �g�p����{�[���s��̃C���f�b�N�X
	Vector4			boneWeight;				// �E�F�C�g�l
	bool operator == (const ModelVertex& v) const {		//�@�������Z�q�̃I�[�o�[���[�h 
		return std::memcmp(this, &v, sizeof(ModelVertex)) == 0;
	}
	ModelVertex() {
		boneWeight = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		position = Vector3(0.0f, 0.0f, 0.0f);
		normal = Vector3(0.0f, 0.0f, 0.0f);
		uv0 = Vector2(0.0f, 0.0f);
	}
};

struct ModelMesh
{
	int							skincount;			// �����X�L���̐�
	std::string					nodeName;			// FBX�m�[�h��
	std::string					materialName;		// �g�p�}�e���A����

	std::vector<ModelVertex>	vertexList;			// ���_���i�[�u�d�b�s�n�q�R���e�i
	std::vector<unsigned int>	indexList;			// �ʂ̍\�����u�d�b�s�n�q�R���e�i
	Matrix4x4					invMeshBaseposeMatrix;	// ���b�V���̏����p��
	std::vector<std::string>	boneNodeNameList;		// �{�[���m�[�h���i�[�u�d�b�s�n�q�R���e�i
	std::vector<Matrix4x4> 		invBoneBaseposeMatrixList;	// �{�[���̏����p���̋t�s��i�[�u�d�b�s�n�q�R���e�i
};

struct ModelMaterial
{
	std::string materialName;
	std::string diffuseTextureName;
	std::string normalTextureName;
	std::string specularTextureName;
	std::string falloffTextureName;
	std::string reflectionMapTextureName;

	Vector4		Diffuse;		//�f�B�t���[�Y
	Vector4		Speqular;		//�X�y�L����
	Vector4		Ambient;		//�A���r�G���g
	float		Power;			//���˂̋���
};