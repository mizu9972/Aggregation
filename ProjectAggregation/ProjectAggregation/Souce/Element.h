#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <memory>
#include <string>
#include <vector>
#include "WindowSetting.h"

using namespace DirectX;

// 頂点構造体定義
struct SimpleVertex
{
	XMFLOAT3 Pos;						// XNA MATHライブラリで使用できるように定義された構造体　float*3 
	XMFLOAT4 Color;						// カラー値
	XMFLOAT3 Normal;					// 法線ベクトル
	XMFLOAT2 Tex;						// テクスチャ座標
	XMFLOAT4 BoneIndex;					// ボーン行列へのインデックス４個分
	XMFLOAT4 BoneWeighs;				// ボーンウェイト４個分
};

// 定数バッファ定義
struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT3 mLightDirection;
	XMFLOAT3 mCameraPos;
};

struct ConstantBufferBoneMatrix			// ボーン行列格納用のコンスタントバッファ
{
	XMMATRIX mBoneMatrix[400];
};

struct ConstantBufferMaterial
{
	XMFLOAT4	diffuse;				// ディフューズ
	XMFLOAT4	specular;				// スペキュラ
	XMFLOAT4	ambient;				// アンビエント
	XMFLOAT4	power;					// パワー値
};


struct sVertex_Point
{
	XMFLOAT3 v;
};


struct Matrix4x4
{
	union
	{
		float   m[16];				// １次元配列
		float	m2[4][4];			// ２次元配列
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
// ボーン情報
struct ModelBoneWeight
{
	unsigned int boneIndex[4];				// ボーンインデックス番号
	Vector4		boneWeight;					// ボーンウエイト値
};

struct sConstantBuffer_Point
{
	XMMATRIX mWVP;
};

struct ModelVertex
{
	Vector3 position;						// 座標
	Vector3 normal;							// 法線情報
	Vector2 uv0;							// テクスチャ座標
	unsigned int	boneIndex[4];			// 使用するボーン行列のインデックス
	Vector4			boneWeight;				// ウェイト値
	bool operator == (const ModelVertex& v) const {		//　＝＝演算子のオーバーロード 
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
	int							skincount;			// ワンスキンの数
	std::string					nodeName;			// FBXノード名
	std::string					materialName;		// 使用マテリアル名

	std::vector<ModelVertex>	vertexList;			// 頂点情報格納ＶＥＣＴＯＲコンテナ
	std::vector<unsigned int>	indexList;			// 面の構成情報ＶＥＣＴＯＲコンテナ
	Matrix4x4					invMeshBaseposeMatrix;	// メッシュの初期姿勢
	std::vector<std::string>	boneNodeNameList;		// ボーンノード名格納ＶＥＣＴＯＲコンテナ
	std::vector<Matrix4x4> 		invBoneBaseposeMatrixList;	// ボーンの初期姿勢の逆行列格納ＶＥＣＴＯＲコンテナ
};

struct ModelMaterial
{
	std::string materialName;
	std::string diffuseTextureName;
	std::string normalTextureName;
	std::string specularTextureName;
	std::string falloffTextureName;
	std::string reflectionMapTextureName;

	Vector4		Diffuse;		//ディフューズ
	Vector4		Speqular;		//スペキュラ
	Vector4		Ambient;		//アンビエント
	float		Power;			//反射の強さ
};