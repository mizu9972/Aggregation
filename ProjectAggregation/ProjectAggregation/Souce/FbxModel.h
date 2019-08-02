#pragma once

#include <fbxsdk.h>
#include <map>
#include "Element.h"

#pragma comment (lib,"libfbxsdk-mt.lib")
#pragma comment (lib, "winmm.lib")
static FbxManager* g_pFbxManager;

class CFbxModel {
private:

	//モデルデータ------------------------------------------------------------------------------------------
	std::vector<ModelMesh> m_MeshList;//ModelMeshのベクターリスト
	std::map<std::string, int> m_NodeIdDictionary;//メッシュ名からMeshList番号を取得するためのmap
	std::vector<ModelMaterial> m_MaterialList;//マテリアルのリスト
	std::map<std::string, int> m_MaterialIdDictionary;//マテリアル名からMateralList番号を取得する為のmap
	//----------------------------------------------------------------------------------------------------

	//Fbxパース--------------------------------------------------------------------------------------------
	FbxScene* m_Scene;//シーン
	FbxImporter* m_Importer;//インポーター
	//----------------------------------------------------------------------------------------------------

	//アニメーション----------------------------------------------------------------------------------------
	FbxScene* m_FbxAnimationScene = nullptr;//アニメーションのシーン
	std::map<std::string, int> m_NodeIdDictionaryAnimation;//ボーン名からノード番号を取得するためのmap
	INT64 m_AnimationFrame_Start = 0;//アニメーション開始フレーム
	INT64 m_AnimationFrame_End = 0;//アニメーション終了フレーム
	UINT m_MotionNo = 0;//モーション番号
	Matrix4x4 m_BoneMatrixList[400];
	INT64 m_Frame = 0;
	//----------------------------------------------------------------------------------------------------

	//描画------------------------------------------------------------------------------------------------
	ID3D11VertexShader* m_pVertexShader = nullptr;//頂点シェーダ
	ID3D11PixelShader* m_pPixelShader = nullptr;//ピクセルシェーダ
	ID3D11InputLayout* m_pVertexLayout = nullptr;//頂点レイアウト

	ID3D11Buffer* m_pVertexBuffer[23];//頂点バッファ
	ID3D11Buffer* m_pIndexBuffer[23];//インデックスバッファ
	ID3D11Buffer* m_pConstantBuffer = nullptr;//コンスタントバッファ
	ID3D11Buffer* m_pConstantBufferBoneMatrix = nullptr;
	ID3D11Buffer* m_pConstantBufferMaterial = nullptr;

	ID3D11SamplerState* m_pSampleLinear = nullptr;//テクスチャサンプラ
	ID3D11ShaderResourceView* m_Texture[23] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	ID3D11Resource* m_pTexResouce = nullptr;

	std::vector<ModelMesh> m_ModelMeshContainer;//MOdelMesh格納用コンテナベクター
	ModelMaterial m_Material[23];
	//----------------------------------------------------------------------------------------------------

	//Loadメソッド用
	void					LoadModel(const char* ModelName_);
	ModelMesh				ParseMesh(FbxMesh* Mesh_);
	ModelMaterial			ParseMaterial(FbxSurfaceMaterial* Material_);
	std::vector<int>		GetIndexList(FbxMesh* Mesh_);
	std::vector<Vector3>	GetPositionList(FbxMesh* Mesh_, const std::vector<int>& IndexList_);
	std::vector<Vector3>	GetNormalList(FbxMesh* Mesh_, const std::vector<int>& IndexList_);
	std::vector<Vector2>	GetUVList(FbxMesh* Mesh_, const std::vector<int>& IndexList_, int UvNo_);
	void					GetWeight(FbxMesh* Mesh_, const std::vector<int>& IndexList_, std::vector<ModelBoneWeight>& BoneWeightList_, std::vector<std::string>& BoneNodeNameList_, std::vector<Matrix4x4>& InvBaseposeMatrixList_);
	bool					CreateVertexBuffer(unsigned int Vertexnum_, void* Vertexdata_, ID3D11Buffer** pVertexBuffer_);
	bool					CreateIndexBuffer(unsigned int Indexnum_, void* Indexdata_, ID3D11Buffer** pIndexBuffer_);
	ModelMaterial&			GetMaterial(std::string MaterialName_);
	bool					createConstantBuffer(unsigned int ByteSize_, ID3D11Buffer** pConstantBuffer_);


public:
	CFbxModel();
	~CFbxModel();

	void Load(const char* ModelName_);//モデル読み込み
	void UnInit();//終了処理
	void LoadFbxAnimation(const char* AnimationFileName_);
	bool isAnimationEnd(INT64 Frame_);//アニメーション終了確認

	void Draw();//描画

	void MakeWorldMatrix(XMMATRIX& mat_, XMFLOAT3& Angle_, XMFLOAT3& Trans_);
	//アクセスメソッド-------------------
	void getBoneMatrix(INT64 Frame_, int MeshId_, Matrix4x4* OutMatrixList, unsigned int MatrixCount_);
	void getMeshmatrix(INT64 Frame_, int MeshId_, Matrix4x4& OutMatrix_);
	std::vector<ModelMesh>& getModelMeshContainer();
	ModelMaterial& getMaterial(std::string MaterialName_);
	//--------------------------------

};