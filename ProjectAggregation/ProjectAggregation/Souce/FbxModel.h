#pragma once

#include <fbxsdk.h>
#include <map>
#include "Element.h"

#pragma comment (lib,"libfbxsdk-mt.lib")
#pragma comment (lib, "winmm.lib")
static FbxManager* g_pFbxManager;

class CFbxModel {
private:

	//���f���f�[�^------------------------------------------------------------------------------------------
	std::vector<ModelMesh> m_MeshList;//ModelMesh�̃x�N�^�[���X�g
	std::map<std::string, int> m_NodeIdDictionary;//���b�V��������MeshList�ԍ����擾���邽�߂�map
	std::vector<ModelMaterial> m_MaterialList;//�}�e���A���̃��X�g
	std::map<std::string, int> m_MaterialIdDictionary;//�}�e���A��������MateralList�ԍ����擾����ׂ�map
	//----------------------------------------------------------------------------------------------------

	//Fbx�p�[�X--------------------------------------------------------------------------------------------
	FbxScene* m_Scene;//�V�[��
	FbxImporter* m_Importer;//�C���|�[�^�[
	//----------------------------------------------------------------------------------------------------

	//�A�j���[�V����----------------------------------------------------------------------------------------
	FbxScene* m_FbxAnimationScene = nullptr;//�A�j���[�V�����̃V�[��
	std::map<std::string, int> m_NodeIdDictionaryAnimation;//�{�[��������m�[�h�ԍ����擾���邽�߂�map
	INT64 m_AnimationFrame_Start = 0;//�A�j���[�V�����J�n�t���[��
	INT64 m_AnimationFrame_End = 0;//�A�j���[�V�����I���t���[��
	UINT m_MotionNo = 0;//���[�V�����ԍ�
	Matrix4x4 m_BoneMatrixList[400];
	INT64 m_Frame = 0;
	//----------------------------------------------------------------------------------------------------

	//�`��------------------------------------------------------------------------------------------------
	ID3D11VertexShader* m_pVertexShader = nullptr;//���_�V�F�[�_
	ID3D11PixelShader* m_pPixelShader = nullptr;//�s�N�Z���V�F�[�_
	ID3D11InputLayout* m_pVertexLayout = nullptr;//���_���C�A�E�g

	ID3D11Buffer* m_pVertexBuffer[23];//���_�o�b�t�@
	ID3D11Buffer* m_pIndexBuffer[23];//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* m_pConstantBuffer = nullptr;//�R���X�^���g�o�b�t�@
	ID3D11Buffer* m_pConstantBufferBoneMatrix = nullptr;
	ID3D11Buffer* m_pConstantBufferMaterial = nullptr;

	ID3D11SamplerState* m_pSampleLinear = nullptr;//�e�N�X�`���T���v��
	ID3D11ShaderResourceView* m_Texture[23] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	ID3D11Resource* m_pTexResouce = nullptr;

	std::vector<ModelMesh> m_ModelMeshContainer;//MOdelMesh�i�[�p�R���e�i�x�N�^�[
	ModelMaterial m_Material[23];
	//----------------------------------------------------------------------------------------------------

	//Load���\�b�h�p
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

	void Load(const char* ModelName_);//���f���ǂݍ���
	void UnInit();//�I������
	void LoadFbxAnimation(const char* AnimationFileName_);
	bool isAnimationEnd(INT64 Frame_);//�A�j���[�V�����I���m�F

	void Draw();//�`��

	void MakeWorldMatrix(XMMATRIX& mat_, XMFLOAT3& Angle_, XMFLOAT3& Trans_);
	//�A�N�Z�X���\�b�h-------------------
	void getBoneMatrix(INT64 Frame_, int MeshId_, Matrix4x4* OutMatrixList, unsigned int MatrixCount_);
	void getMeshmatrix(INT64 Frame_, int MeshId_, Matrix4x4& OutMatrix_);
	std::vector<ModelMesh>& getModelMeshContainer();
	ModelMaterial& getMaterial(std::string MaterialName_);
	//--------------------------------

};