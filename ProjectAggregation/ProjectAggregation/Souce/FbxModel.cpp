
#include "Shader.h"
#include "DX11util.h"
#include "Utility.h"
#include "FbxModel.h"

//�P�ʍs��
Matrix4x4 Matrix4x4Identity = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

XMMATRIX g_World;
XMMATRIX g_View;
XMMATRIX g_Projection;

CFbxModel::CFbxModel() {
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pVertexLayout = nullptr;
};

CFbxModel::~CFbxModel() {

};

void CFbxModel::Load(const char* ModelName_) {
	//Fbx���f���ǂݍ���

	//���_�V�F�[�_�[���R���p�C��
	//ID3DBlob* pVSBlob = nullptr;
	bool hr/* = CompileShaderFromFile("Shader/fbxtest.fx", "main", "vs_4_0", &pVSBlob)*/;

	//if (FAILED(hr)) {
	//	MessageBox(nullptr,
	//		"FX file compiled error.", "Error", MB_OK);

	//}

	//���_���C�A�E�g�ݒ�
	D3D11_INPUT_ELEMENT_DESC layout[] = {

		{ "POSITION",        0,      DXGI_FORMAT_R32G32B32_FLOAT,      0,      0,										D3D11_INPUT_PER_VERTEX_DATA, 0 },	// ���_���W
		{ "COLOR",           0,      DXGI_FORMAT_R32G32B32A32_FLOAT,   0,      4 * 3,									D3D11_INPUT_PER_VERTEX_DATA, 0 },	// ���_�J���[
		{ "NORMAL",          0,      DXGI_FORMAT_R32G32B32_FLOAT,      0,      4 * 4 + 4 * 3 ,							D3D11_INPUT_PER_VERTEX_DATA, 0 },	// �@���x�N�g��
		{ "TEXCOORD",        0,      DXGI_FORMAT_R32G32_FLOAT,         0,      4 * 3 + 4 * 4 + 4 * 3,					D3D11_INPUT_PER_VERTEX_DATA, 0 },	// �e�N�X�`�����W
		{ "BONEINDEX",		 0,		 DXGI_FORMAT_R32G32B32A32_FLOAT,   0,	   4 * 2 + 4 * 3 + 4 * 4 + 4 * 3,			D3D11_INPUT_PER_VERTEX_DATA, 0 },	// �{�[���s��C���f�N�b�X
		{ "BONEWEIGHT",		 0,		 DXGI_FORMAT_R32G32B32A32_FLOAT,   0,	   4 * 4 + 4 * 2 + 4 * 3 + 4 * 4 + 4 * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },	// �{�[���E�F�C�g
	};
	unsigned int numElements = ARRAYSIZE(layout);
	//���_�V�F�[�_�[�쐬

	bool sts = CreateVertexShader(
		GetDX11Device(),
		"Shader/fbxtest.fx",
		"main",
		"vs_4_0",
		layout,
		numElements,
		&m_pVertexShader,
		&m_pVertexLayout
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return;
	}
	//���_�f�[�^���f�o�C�X�ɃZ�b�g
	GetDX11DeviceContext()->IASetInputLayout(m_pVertexLayout);


	//�s�N�Z���V�F�[�_�[�쐬
	sts = CreatePixelShader(
		GetDX11Device(),
		"Shader/fbxtest.fx",
		"PS", 
		"ps_4_0",
		&m_pPixelShader
		);

	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return;
	}


	////���_�V�F�[�_�[�쐬
	//hr = GetDX11Device()->CreateVertexShader(
	//	pVSBlob->GetBufferPointer(),
	//	pVSBlob->GetBufferSize(),
	//	nullptr,
	//	&m_pVertexShader
	//);
	//if (FAILED(hr)) {
	//	pVSBlob->Release();
	//}
	////���_���C�A�E�g����
	//hr = GetDX11Device()->CreateInputLayout(
	//	layout,
	//	numElements,
	//	pVSBlob->GetBufferPointer(),
	//	pVSBlob->GetBufferSize(),
	//	&m_pVertexLayout
	//);
	//if (FAILED(hr)) {
	//	MessageBox(nullptr, "Create Layout Error", "error", MB_OK);
	//}

	////���
	//pVSBlob->Release();


	////�s�N�Z���V�F�[�_�[���R���p�C��
	//ID3DBlob* pPSBlob = nullptr;
	//hr = CompileShaderFromFile("Shader/fbxtest.fx", "PS", "ps_4_0", &pPSBlob);
	//if (FAILED(hr)) {
	//	MessageBox(nullptr, "Pixel Shader Error", "error", MB_OK);

	//}

	////�s�N�Z���V�F�[�_����
	//hr = GetDX11Device()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	//if (FAILED(hr)) {
	//	MessageBox(nullptr, "Create Pixelshader Error", "error", MB_OK);

	//}
	//pPSBlob->Release();

	//���f���ǂݍ���
	LoadModel(ModelName_);

	//���_�o�b�t�@����
	for (unsigned int i = 0; i < m_MeshList.size(); i++) {
		SimpleVertex* v;
		v = new SimpleVertex[m_MeshList[i].vertexList.size()];

		for (unsigned int j = 0; j < m_MeshList[i].vertexList.size(); j++) {

			//���_���W���Z�b�g
			v[j].Pos.x = m_MeshList[i].vertexList[j].position.x;
			v[j].Pos.y = m_MeshList[i].vertexList[j].position.y;
			v[j].Pos.z = m_MeshList[i].vertexList[j].position.z;

			//���_�J���[���Z�b�g
			v[j].Color.w = 1.0f;
			v[j].Color.x = 1.0f;
			v[j].Color.y = 1.0f;
			v[j].Color.z = 1.0f;

			//�@���x�N�g�����Z�b�g
			v[j].Normal.x = m_MeshList[i].vertexList[j].normal.x;
			v[j].Normal.y = m_MeshList[i].vertexList[j].normal.y;
			v[j].Normal.z = m_MeshList[i].vertexList[j].normal.z;

			//�e�N�X�`�����W���Z�b�g
			v[j].Tex.x = m_MeshList[i].vertexList[j].uv0.x;
			v[j].Tex.y = 1.0f - m_MeshList[i].vertexList[j].uv0.y;

			//�{�[���s��C���f�b�N�X���Z�b�g
			v[j].BoneIndex.w = m_MeshList[i].vertexList[j].boneIndex[0];
			v[j].BoneIndex.x = m_MeshList[i].vertexList[j].boneIndex[1];
			v[j].BoneIndex.y = m_MeshList[i].vertexList[j].boneIndex[2];
			v[j].BoneIndex.z = m_MeshList[i].vertexList[j].boneIndex[3];

			//�{�[���E�F�C�g�l���Z�b�g
			v[j].BoneWeighs.w = m_MeshList[i].vertexList[j].boneWeight.v[0];
			v[j].BoneWeighs.x = m_MeshList[i].vertexList[j].boneWeight.v[1];
			v[j].BoneWeighs.y = m_MeshList[i].vertexList[j].boneWeight.v[2];
			v[j].BoneWeighs.z = m_MeshList[i].vertexList[j].boneWeight.v[3];
		}

		//���_�o�b�t�@����
		bool sts = CreateVertexBuffer(
			m_MeshList[i].vertexList.size(),
			v,
			&m_pVertexBuffer[i]
			);

		if (!sts) {
			MessageBox(nullptr, "Create Vertex Buffer Error", "error", MB_OK);

		}
		delete[] v;
	}

	//�C���f�b�N�X�o�b�t�@�쐬
	for (unsigned int i = 0; i < m_MeshList.size(); i++) {
		WORD* indexList;

		//�C���f�b�N�X���擾
		indexList = new WORD[m_MeshList[i].indexList.size()];

		//�C���f�b�N�X�f�[�^�Z�b�g
		for (unsigned int j = 0; j < m_MeshList[i].indexList.size(); j++) {
			indexList[j] = m_MeshList[i].indexList[j];
		}

		//�C���f�b�N�X�o�b�t�@�쐬
		bool sts = CreateIndexBuffer(
			m_MeshList[i].indexList.size(),
			indexList,
			&m_pIndexBuffer[i]
		);

		if (!sts) {
			MessageBox(nullptr, "Create Index Buffer Error", "error", MB_OK);

		}

		delete[] indexList;
	}

	//�V�F�[�_�[���\�[�X�r���[����
	for (unsigned int i = 0; i < m_MeshList.size(); i++) {
		std::string TexturePath;
		std::string MaterailName = m_MeshList[i].materialName;
		if (MaterailName == "noName") {
			TexturePath = "assets/model/texture/PBR_Free_Albedo.tga";
		}
		else {
			m_Material[i] = GetMaterial(m_MeshList[i].materialName);
		}
		std::string Textures = ("assets/model/texture/");
		TexturePath = std::string("assets/model/texture/" + m_Material[i].diffuseTextureName);
		int PathLength = TexturePath.size() - Textures.size();
		if (PathLength <= 0) {
			TexturePath = "assets/model/texture/PBR_Free_Albedo.tga";
		}

		//�e�N�X�`���ǂݍ���
		const char *ms = TexturePath.c_str();
		wchar_t ws[512];
		size_t ret;

		setlocale(LC_CTYPE, "jpn");
		mbstowcs_s(&ret, ws, 512, ms, _TRUNCATE);

		DirectX::TexMetadata meta;
		DirectX::GetMetadataFromTGAFile(ws, meta);

		std::unique_ptr<DirectX::ScratchImage> image(new DirectX::ScratchImage);
		hr = LoadFromTGAFile(ws, &meta, *image);
		if (FAILED(hr))
		{
			MessageBox(nullptr, "Texture Load error", "error", MB_OK);
		}

		//�V�F�[�_�[���\�[�X�r���[�쐬
		hr = DirectX::CreateShaderResourceView(GetDX11Device(), image->GetImages(), image->GetImageCount(), meta, &m_Texture[i]);
		if (FAILED(hr)) {
			MessageBox(nullptr, "Create ShaderResouceView Error", "error", MB_OK);
		}
	}

	//�R���X�^���g�o�b�t�@�쐬
	sts = createConstantBuffer(
		sizeof(ConstantBuffer),
		&m_pConstantBuffer
	);
	if (!sts) {
		MessageBox(nullptr, "Create Constant Buffer Error", "error", MB_OK);
	}

	//�{�[���s��p�R���X�^���g�o�b�t�@�쐬
	sts = createConstantBuffer(
		sizeof(ConstantBuffer),
		&m_pConstantBufferBoneMatrix
	);
	if (!sts) {
		MessageBox(nullptr, "Create Constant Buffer Error", "error", MB_OK);
	}

	//�{�[���}�e���A���p�R���X�^���g�o�b�t�@�쐬
	sts = createConstantBuffer(
		sizeof(ConstantBuffer),
		&m_pConstantBufferMaterial
	);
	if (!sts) {
		MessageBox(nullptr, "Create Constant Buffer Error", "error", MB_OK);
	}

	//���[���h�ϊ��s�񏉊���
	g_World = XMMatrixIdentity();

	XMVECTOR Eye = XMVectorSet(0.0f, 100.0f, -100.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	// �v���W�F�N�V�����ϊ��s�񏉊���
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800 / (FLOAT)600, 0.01f, 10000.0f);

	// �e�N�X�`���p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamplerDesc;
	ZeroMemory(&SamplerDesc, sizeof(D3D11_SAMPLER_DESC));

	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	GetDX11Device()->CreateSamplerState(&SamplerDesc, &m_pSampleLinear);

};

void CFbxModel::LoadModel(const char* Modelname_) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//FbxManager����
	g_pFbxManager = FbxManager::Create();
	if (!g_pFbxManager) {
		//error
		exit(1);
	}

	FbxIOSettings* ios = FbxIOSettings::Create(g_pFbxManager, IOSROOT);
	g_pFbxManager->SetIOSettings(ios);

	//�C���|�[�^�[�쐬
	m_Importer = FbxImporter::Create(g_pFbxManager, "");
	if (!m_Importer->Initialize(Modelname_, -1, g_pFbxManager->GetIOSettings())) {
		exit(1);
	}
	//�V�[���쐬
	m_Scene = FbxScene::Create(g_pFbxManager, "Scene");
	if (!m_Scene) {
		//error
		exit(1);
	}

	//Fbx���p�[�X���ăV�[���ɓǂݍ���
	bool sts = m_Importer->Import(m_Scene);
	if (!sts) {
		exit(1);
	}

	//�C���|�[�^�[���
	m_Importer->Destroy();

	//�|���S�����O�p�`��
	FbxGeometryConverter geometryConverter(g_pFbxManager);
	geometryConverter.Triangulate(m_Scene, true);

	//���b�V���̉��
	int MeshCount = m_Scene->GetMemberCount<FbxMesh>();
	printf("���b�V���̐� %d \n", MeshCount);

	int NodeCount = m_Scene->GetNodeCount();
	printf("�m�[�h�̐� %d \n", NodeCount);

	//�m�[�h���������ɓo�^
	for (int i = 0; i < NodeCount; ++i) {
		FbxNode* fbxNode = m_Scene->GetNode(i);
		m_NodeIdDictionary.insert(std::pair<std::string, int>(fbxNode->GetName(), i));
	}

	//�m�[�h�̒��g���m�F
	for (std::map<std::string, int>::iterator it = m_NodeIdDictionary.begin(); it != m_NodeIdDictionary.end(); ++it) {
		//		std::cout << "key = " << (*it).first << ",value = " << (*it).second << std::endl;
	}

	//�V�[���Ɋ܂܂��}�e���A���̉��
	static int materialCount = m_Scene->GetMaterialCount();
	
	m_MaterialList.reserve(materialCount);
	for (int i = 0; i < materialCount; i++) {
		FbxSurfaceMaterial* fbxMaterial = m_Scene->GetMaterial(i);

		ModelMaterial modelMaterial = ParseMaterial(fbxMaterial);
		m_MaterialList.push_back(modelMaterial);
		m_MaterialIdDictionary.insert(std::pair<std::string, int>(modelMaterial.materialName,i));
	}

	//�V�[���Ɋ܂܂�郁�b�V���̉��
	MeshCount = m_Scene->GetMemberCount<FbxMesh>();
	m_MeshList.reserve(MeshCount);

	for (int i = 0; i < MeshCount; ++i) {
		FbxMesh* fbxMesh = m_Scene->GetMember<FbxMesh>(i);
		m_MeshList.push_back(ParseMesh(fbxMesh));
	}
};

void CFbxModel::UnInit() {

};

void CFbxModel::getBoneMatrix(INT64 Frame_, int MeshId_, Matrix4x4* OutMatrixList, unsigned int MatrixCount_) {
	ModelMesh& modelMesh = m_MeshList[MeshId_];

	if (modelMesh.boneNodeNameList.size() == 0) {//�{�[�������X�g
		OutMatrixList[0] = Matrix4x4Identity;
		return;
	}

	assert(modelMesh.boneNodeNameList.size() <= MatrixCount_);//�{�[�������z�񐔂�葽���ꍇ�I��

	FbxTime time;
	time.Set(FbxTime::GetOneFrameValue(FbxTime::eFrames60) * Frame_); //�o�ߎ��Ԃɍ��킹�ăt���[���Z�b�g

	//���̃��b�V���Ɋ܂܂�Ă���{�[���������[�v
	for (unsigned int i = 0; i < modelMesh.boneNodeNameList.size(); i++) {
		std::string& boneNodeName = modelMesh.boneNodeNameList[i];
		int boneNodeId = m_NodeIdDictionaryAnimation.at(boneNodeName);
		FbxNode* boneNode = m_FbxAnimationScene->GetNode(boneNodeId);

		FbxMatrix boneMatrix = boneNode->EvaluateGlobalTransform(time);		// �o�ߎ��Ԃɉ������p��������킷�{�[���s����擾
		Matrix4x4& out_matrix = OutMatrixList[i];

		// FbxMatrix�idouble�̔z��j����Matrix4x4(float�̔z��)�֕ϊ�
		double* boneMatrixPtr = (double*)boneMatrix;
		for (int j = 0; j < 16; ++j)
		{
			out_matrix.m[j] = static_cast<float>(boneMatrixPtr[j]);			// double->float
		}

		MulMatrixMatrix(modelMesh.invBoneBaseposeMatrixList[i], out_matrix, out_matrix);		// �{�[���I�t�Z�b�g�s�񁖃{�[���s��

	}
}

void CFbxModel::getMeshmatrix(INT64 Frame_, int MeshId_, Matrix4x4& OutMatrix_) {
	ModelMesh& modelMesh = m_MeshList[MeshId_];

	std::map<std::string, int>::iterator it = m_NodeIdDictionaryAnimation.find(modelMesh.nodeName);		// ���b�V���̃m�[�h������m�[�h�h�c���擾
	// ������Ȃ������ꍇ
	if (it == m_NodeIdDictionaryAnimation.end())
	{
		OutMatrix_ = Matrix4x4Identity;
		return;
	}

	int meshNodeId = it->second;														// �m�[�h�h�c�擾
	FbxNode* meshNode = m_FbxAnimationScene->GetNode(meshNodeId);				// FBX�m�[�h���擾

	FbxTime time;
	time.Set(FbxTime::GetOneFrameValue(FbxTime::eFrames60) * Frame_);		// �w��t���[���ɉ������o�ߎ��Ԃ��Z�b�g

	FbxMatrix meshMatrix = meshNode->EvaluateGlobalTransform(time);			// �o�ߎ��Ԃɉ������p��������킷�s����擾

	// FbxMatrix�idouble�̔z��j����Matrix4x4(float�̔z��)�֕ϊ�
	double* meshMatrixPtr = (double*)meshMatrix;
	for (int i = 0; i < 16; ++i)
	{
		OutMatrix_.m[i] = static_cast<float>(meshMatrixPtr[i]);				// double->float
	}

	MulMatrixMatrix(modelMesh.invMeshBaseposeMatrix, OutMatrix_, OutMatrix_);		// �I�t�Z�b�g�s�񁖃��b�V���p���s��

}

void CFbxModel::LoadFbxAnimation(const char* AnimationFileName_) {
	// ���łɃA�j���[�V������ǂݍ���ł�����
	if (m_FbxAnimationScene != nullptr)
	{
		m_FbxAnimationScene->Destroy();
		m_FbxAnimationScene = nullptr;
		m_NodeIdDictionaryAnimation.clear();
	}

	//�A�j���[�V�������擾
	//Importer����
	m_Importer = FbxImporter::Create(g_pFbxManager, "");

	if (!m_Importer->Initialize(AnimationFileName_, -1, g_pFbxManager->GetIOSettings()))
	{
		//		printf("Error: Unable to create FBX m_Importer(animation)!\n");
		exit(1);
	}

	//animationScene����
	m_FbxAnimationScene = FbxScene::Create(g_pFbxManager, "animationScene");
	if (!m_FbxAnimationScene)
	{
		//		printf("Error: Unable to create FBX scene(animation)!\n");
		exit(1);
	}
	else
	{
		//		printf("create FBX scene(animation) success\n");
	}

	//FBX���p�[�X����m_FbxAnimationScene�ɓǂݍ���
	bool sts = m_Importer->Import(m_FbxAnimationScene);
	if (!sts)
	{
		//		printf("Error: Unable m_Importer scene!\n");
		exit(1);
	}
	else
	{
		//		printf("Import Scene success\n");
	}

	//�A�j���[�V�������擾
	int animStackCount = m_Importer->GetAnimStackCount();				// �A�j���[�V���������擾
	//assert(animStackCount == 1);									// �P�A�j���[�V���������ɑΉ�
	FbxTakeInfo* takeInfo = m_Importer->GetTakeInfo(0);				// �O�Ԗڂ̂s�`�j�d���擾

	FbxTime importOffset = takeInfo->mImportOffset;					// �I�t�Z�b�g����
	FbxTime startTime = takeInfo->mLocalTimeSpan.GetStart();		// �J�n�������擾
	FbxTime stopTime = takeInfo->mLocalTimeSpan.GetStop();			// �I���������擾

	// �A�j���[�V�����J�n���Ԏ��ԁ^�P�t���[���̎��ԂŊJ�n�t���[�������߂Ă���
	m_AnimationFrame_Start = (importOffset.Get() + startTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::eFrames60);
	// �A�j���[�V�����I�����ԁ^�P�t���[���̎��Ԃ̎��ԂŏI���t���[�������߂Ă���
	m_AnimationFrame_End = (importOffset.Get() + stopTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::eFrames60);
	m_Importer->Destroy();

	// �m�[�h������m�[�hID���擾�ł���悤�Ɏ����ɓo�^
	int nodeCount = m_FbxAnimationScene->GetNodeCount();
	printf("animationNodeCount: %d\n", nodeCount);
	for (int i = 0; i < nodeCount; ++i)
	{
		auto fbxNode = m_FbxAnimationScene->GetNode(i);
		m_NodeIdDictionaryAnimation.insert(std::pair<std::string, int>(fbxNode->GetName(), i));
	}
}

std::vector<ModelMesh>& CFbxModel::getModelMeshContainer() {
	std::vector<ModelMesh> ret;
	return ret;
}

ModelMaterial& CFbxModel::GetMaterial(std::string MaterialName_) {
	// �}�e���A�����ŃC���f�b�N�X���T�[�`
	std::map<std::string, int>::iterator it = m_MaterialIdDictionary.find(MaterialName_);
	return m_MaterialList[(*it).second];
}

bool CFbxModel::isAnimationEnd(INT64 Frame_) {
	///
	return false;
}

ModelMesh CFbxModel::ParseMesh(FbxMesh* Mesh_) {
	assert(Mesh_ != nullptr);

	FbxNode* fbxnode = Mesh_->GetNode();					// �e�a�w�m�[�h���擾

	// �}�e���A����1�̂��̂����Ή����Ȃ�

	int materialCount = fbxnode->GetMaterialCount();
	//assert(fbxnode->GetMaterialCount() == 1);

	ModelMesh modelMesh;								// �ŏI�I�ɕۑ�����郁�b�V�����i���̊֐��̏o�͒l�j

	// �����X�L���̐����擾
	modelMesh.skincount = Mesh_->GetDeformerCount(FbxDeformer::eSkin);		// �����X�L�����̐����Z�b�g
	modelMesh.nodeName = fbxnode->GetName();	// �e�a�w�m�[�h�����Z�b�g
	if (materialCount > 0)
	{
		modelMesh.materialName = fbxnode->GetMaterial(0)->GetName();			// �}�e���A�������Z�b�g
	}
	else
	{
		modelMesh.materialName = "noName";
	}
	printf("Mesh_:%s ,%s \n", modelMesh.nodeName.c_str(), modelMesh.materialName.c_str());

	// �x�[�X�|�[�Y�̋t�s����쐬���Ă���(���b�V����)
	FbxMatrix baseposeMatrix = fbxnode->EvaluateGlobalTransform().Inverse();

	double* baseposeMatrixPtr = (double*)baseposeMatrix;

	for (int i = 0; i < 16; ++i)
	{
		modelMesh.invMeshBaseposeMatrix.m[i] = static_cast<float>(baseposeMatrixPtr[i]);
	}

	// �C���f�b�N�X�u�d�b�s�n�q�擾
	std::vector<int> indexList = GetIndexList(Mesh_);

	// ���_�f�[�^�擾
	std::vector<Vector3> positionList = GetPositionList(Mesh_, indexList);		// ���_���W���擾�i�ʂ̍\�����ɍ��킹��j
	std::vector<Vector3> normalList = GetNormalList(Mesh_, indexList);			// �@���x�N�g�����擾�i�ʂ̍\�����ɍ��킹��j
	std::vector<Vector2> uv0List = GetUVList(Mesh_, indexList, 0);				// UV���W���擾�i�ʂ̍\�����ɍ��킹��j

	// �E�G�C�g�l���擾����
	std::vector<ModelBoneWeight> boneWeightList;
	GetWeight(Mesh_,											// ���b�V���i���́j
		indexList,										// �ʂ̍\�����i���́j
		boneWeightList,									// �{�[����񃊃X�g�i�o�͒l�j�i�{�[�����ւ̃C���f�b�N�X�l�ƃE�F�C�g�l�j
		modelMesh.boneNodeNameList,						// ���b�V�����̃{�[�������u�d�b�s�n�q�ɕۑ��i�o�́j
		modelMesh.invBoneBaseposeMatrixList);			// �{�[���I�t�Z�b�g�s����u�d�b�s�n�q�ɕۑ��i�j

// �e���|�����̒��_���i�l���������u�����������j�����
	std::vector<ModelVertex> tmpmodelVertexList;
	tmpmodelVertexList.reserve(indexList.size());		// ���_�C���f�b�N�X�T�C�Y���e�ʂ��m��

	// ���_�C���f�b�N�X�����i�ʂ̍\����񏇁j���[�v����
	for (unsigned int i = 0; i < indexList.size(); ++i)
	{
		ModelVertex vertex;
		vertex.position = positionList[i];			// ���_���

		if (normalList.size() != 0)
		{
			vertex.normal = normalList[i];				// �@�����
		}

		if (uv0List.size() == 0)
		{
			vertex.uv0 = Vector2(0.0f, 0.0f);
		}
		else
		{
			vertex.uv0 = uv0List[i];
		}

		// �{�[���E�F�C�g�l�����݂��邩�H
		if (boneWeightList.size() > 0)
		{
			// �{�[���̃C���f�b�N�X�ԍ����Z�b�g
			for (int j = 0; j < 4; ++j)
			{
				vertex.boneIndex[j] = boneWeightList[i].boneIndex[j];
			}
			// �E�F�C�g�l���Z�b�g
			vertex.boneWeight = boneWeightList[i].boneWeight;
		}
		else
		{
			for (int j = 0; j < 4; ++j)
			{
				vertex.boneIndex[j] = 0;
			}
			vertex.boneWeight = Vector4(1.0, 0.0, 0.0, 0.0);
		}

		tmpmodelVertexList.push_back(vertex);			// 
	}

	// �d�����_�������ăC���f�b�N�X�쐬
	modelMesh.vertexList.reserve(tmpmodelVertexList.size());		// ���_��񐔕��G���A���m�ۂ���

	modelMesh.indexList.reserve(indexList.size());		// �ʂ̍\����񐔕��G���A���m�ۂ���

	// �����ŏd�����Ă��钸�_�f�[�^�������A���_�f�[�^���X�g�ƃC���f�b�N�X���X�g����蒼���Ă���
	// �d���������_���܂񂾒��_�������[�v���Ă���(�ʂ̍\���������ɔ����o�������_���)
	for (unsigned int i = 0; i < tmpmodelVertexList.size(); i++)
	{
		ModelVertex& vertex = tmpmodelVertexList[i];
		// �d�����Ă��邩�H
		// modelMesh.vertexList�́A�ŏ���ł��񂾂�o�^����Ă����i�d�����Ă��Ȃ����_���Ƃ��āj
		auto it = std::find(modelMesh.vertexList.begin(), modelMesh.vertexList.end(), vertex);
		if (it == modelMesh.vertexList.end())
		{
			// �d�����Ă��Ȃ�
			modelMesh.indexList.push_back(modelMesh.vertexList.size());	// ���_�C���f�b�N�X���i�[
			modelMesh.vertexList.push_back(vertex);					// ���_�����i�[
		}
		else
		{
			// �d�����Ă���
			auto index = std::distance(modelMesh.vertexList.begin(), it);	// �擪���猻�C�e���[�^�i�d�����_�����擪�f�[�^���w�������Ă���j�܂ł̃C���f�b�N�X�ԍ����擾
			modelMesh.indexList.push_back(index);// �C���f�b�N�X�ԍ��i�d�����_�����擪�f�[�^���w�������Ă���j���C���f�b�N�X���X�g�ɃZ�b�g
		}
	}

	return modelMesh;

}

ModelMaterial CFbxModel::ParseMaterial(FbxSurfaceMaterial* Material_) {
	ModelMaterial modelMaterial;
	modelMaterial.materialName = Material_->GetName();

	printf("Material_ name :%s \n", modelMaterial.materialName.c_str());

	enum eMATERIAL_TYPE
	{
		MATERIAL_LAMBERT,
		MATERIAL_PHONG
	};

	eMATERIAL_TYPE material_type;

	// �}�e���A�����擾
	if (Material_->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		// lambert
		material_type = MATERIAL_LAMBERT;
		FbxSurfaceLambert* pLambert = (FbxSurfaceLambert*)Material_;
		modelMaterial.Diffuse.v[0] = (float)pLambert->Diffuse.Get()[0];
		modelMaterial.Diffuse.v[1] = (float)pLambert->Diffuse.Get()[1];
		modelMaterial.Diffuse.v[2] = (float)pLambert->Diffuse.Get()[2];
		modelMaterial.Diffuse.v[3] = 1.0f;

		modelMaterial.Ambient.v[0] = (float)pLambert->Ambient.Get()[0];
		modelMaterial.Ambient.v[1] = (float)pLambert->Ambient.Get()[1];
		modelMaterial.Ambient.v[2] = (float)pLambert->Ambient.Get()[2];

		modelMaterial.Speqular.v[0] = 0.0f;
		modelMaterial.Speqular.v[1] = 0.0f;
		modelMaterial.Speqular.v[2] = 0.0f;
		modelMaterial.Speqular.v[3] = 1.0f;

		modelMaterial.Power = 1.0f;
	}
	else if (Material_->GetClassId().Is(FbxSurfacePhong::ClassId)) {
		// phong
		material_type = MATERIAL_PHONG;
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)(Material_);
		modelMaterial.Diffuse.v[0] = (float)pPhong->Diffuse.Get()[0];
		modelMaterial.Diffuse.v[1] = (float)pPhong->Diffuse.Get()[1];
		modelMaterial.Diffuse.v[2] = (float)pPhong->Diffuse.Get()[2];
		modelMaterial.Diffuse.v[3] = 1.0f;

		modelMaterial.Ambient.v[0] = pPhong->Ambient.Get()[0];
		modelMaterial.Ambient.v[1] = pPhong->Ambient.Get()[1];
		modelMaterial.Ambient.v[2] = pPhong->Ambient.Get()[2];
		modelMaterial.Ambient.v[3] = 1.0f;

		modelMaterial.Speqular.v[0] = pPhong->Specular.Get()[0];
		modelMaterial.Speqular.v[1] = pPhong->Specular.Get()[1];
		modelMaterial.Speqular.v[2] = pPhong->Specular.Get()[2];
		modelMaterial.Speqular.v[3] = pPhong->Specular.Get()[3];

		modelMaterial.Power = pPhong->Shininess;
	}
	else {
		modelMaterial.Diffuse.v[0] = 1.0f;
		modelMaterial.Diffuse.v[1] = 1.0f;
		modelMaterial.Diffuse.v[2] = 1.0f;
		modelMaterial.Diffuse.v[3] = 1.0f;

		modelMaterial.Ambient.v[0] = 1.0f;
		modelMaterial.Ambient.v[1] = 1.0f;
		modelMaterial.Ambient.v[2] = 1.0f;
		modelMaterial.Ambient.v[3] = 1.0f;

		modelMaterial.Speqular.v[0] = 0.0f;
		modelMaterial.Speqular.v[1] = 0.0f;
		modelMaterial.Speqular.v[2] = 0.0f;
		modelMaterial.Speqular.v[3] = 1.0f;

		modelMaterial.Power = 0.0f;
	}

	printf("material diffuse:%f %f %f \n", modelMaterial.Diffuse.v[0], modelMaterial.Diffuse.v[1], modelMaterial.Diffuse.v[2]);
	printf("material specular:%f %f %f \n", modelMaterial.Speqular.v[0], modelMaterial.Speqular.v[1], modelMaterial.Speqular.v[2]);

	const FbxProperty propertyb = Material_->FindProperty(FbxSurfaceMaterial::sEmissive);
	const FbxProperty Factory = Material_->FindProperty(FbxSurfaceMaterial::sEmissive);

	// �e�N�X�`�����擾
	auto implementation = GetImplementation(Material_, FBXSDK_IMPLEMENTATION_CGFX);
	//assert(implementation != nullptr);

	//�擾�ł��Ȃ�������^�������f����`�悷��
	if (implementation != nullptr)
	{
		auto rootTable = implementation->GetRootTable();
		auto entryCount = rootTable->GetEntryCount();

		for (unsigned int i = 0; i < entryCount; ++i)
		{
			auto entry = rootTable->GetEntry(i);
			auto fbxProperty = Material_->FindPropertyHierarchical(entry.GetSource());
			if (!fbxProperty.IsValid())
			{
				fbxProperty = Material_->RootProperty.FindHierarchical(entry.GetSource());
			}

			auto textureCount = fbxProperty.GetSrcObjectCount<FbxTexture>();
			if (textureCount > 0) {
				std::string src = entry.GetSource();

				for (int j = 0; j < fbxProperty.GetSrcObjectCount<FbxFileTexture>(); ++j)
				{
					auto tex = fbxProperty.GetSrcObject<FbxFileTexture>(j);
					std::string texName = tex->GetFileName();
					texName = texName.substr(texName.find_last_of('/') + 1);

					if (src == "Maya|DiffuseTexture")
					{
						modelMaterial.diffuseTextureName = texName;
					}
					else if (src == "Maya|NormalTexture")
					{
						modelMaterial.normalTextureName = texName;
					}
					else if (src == "Maya|SpecularTexture")
					{
						modelMaterial.specularTextureName = texName;
					}
					else if (src == "Maya|FalloffTexture")
					{
						modelMaterial.falloffTextureName = texName;
					}
					else if (src == "Maya|ReflectionMapTexture")
					{
						modelMaterial.reflectionMapTextureName = texName;
					}
				}
			}
		}

		//	printf("diffuseTexture: %s\n",modelMaterial.diffuseTextureName.c_str());
		//	printf("normalTexture: %s\n", modelMaterial.normalTextureName.c_str());
		//	printf("specularTexture: %s\n", modelMaterial.specularTextureName.c_str());
		//	printf("falloffTexture: %s\n", modelMaterial.falloffTextureName.c_str());
		//	printf("reflectionMapTexture: %s\n", modelMaterial.reflectionMapTextureName.c_str());


		//	FILE* fp;
		//	fopen_s(&fp,"texlist.txt","a");

		//	fprintf(fp,"diffuseTexture: %s\n", modelMaterial.diffuseTextureName.c_str());

		//	fclose(fp);

	}

	return modelMaterial;
}

std::vector<int> CFbxModel::GetIndexList(FbxMesh* Mesh_) {
	int polygonCount = Mesh_->GetPolygonCount();			// �O�p�`�̐����擾
	std::vector<int> indexList;							// ���_�C���f�b�N�X�i�ʂ̍\�����j
	indexList.reserve(polygonCount * 3);				// 3�p�`�̐��~�R

	// �ʂ̍\�������擾����
	for (int i = 0; i < polygonCount; i++)
	{
		indexList.push_back(Mesh_->GetPolygonVertex(i, 0));		// i�Ԗڂ̎O�p�`�̂O�Ԗڂ̒��_�C���f�b�N�X���擾
		indexList.push_back(Mesh_->GetPolygonVertex(i, 1));		// i�Ԗڂ̎O�p�`�̂P�Ԗڂ̒��_�C���f�b�N�X���擾
		indexList.push_back(Mesh_->GetPolygonVertex(i, 2));		// i�Ԗڂ̎O�p�`�̂Q�Ԗڂ̒��_�C���f�b�N�X���擾
	}
	return indexList;
}

std::vector<Vector3> CFbxModel::GetPositionList(FbxMesh* Mesh_, const std::vector<int>& IndexList_) {
	//�@���_���W���擾
	std::vector<Vector3> positionList;					//	���_���W�i�[�u�d�b�s�n�q 
	positionList.reserve(IndexList_.size());				//  

	for (unsigned int i = 0; i < IndexList_.size(); i++) {
		int index = IndexList_[i];						// i�Ԗڂ̒��_���i�ʂ̍\�����񏇂ɕ���ł���j���擾
		FbxVector4 Position = Mesh_->GetControlPointAt(index);		// index�Ԗڂ̒��_���W���擾
		if (Position[3] != 0.0) {
			if (Position[3] == 1.0) {
				positionList.push_back(Vector3(static_cast<float>(Position[0]), static_cast<float>(Position[1]), static_cast<float>(Position[2])));			// �������W��
			}
			else {
				positionList.push_back(Vector3(static_cast<float>(Position[0] / Position[3]), static_cast<float>(Position[1] / Position[3]), static_cast<float>(Position[2] / Position[3])));		// �������W��
			}
		}
		else {
			positionList.push_back(Vector3(static_cast<float>(Position[0]), static_cast<float>(Position[1]), static_cast<float>(Position[2])));				// �v�𖳎����Ċi�[
		}
	}
	return positionList;
}

std::vector<Vector3> CFbxModel::GetNormalList(FbxMesh* Mesh_, const std::vector<int>& IndexList_) {
	int elementCount = Mesh_->GetElementNormalCount();			// ���̖@����񂪃Z�b�g����Ă��邩

	assert(elementCount == 1);					// �P�̖@�����ɂ����Ή����Ȃ�

	FbxGeometryElementNormal* element = Mesh_->GetElementNormal(0);	// �O�Ԗڂ̖@���Z�b�g���擾
	FbxLayerElement::EMappingMode mappingmode = element->GetMappingMode();	// �}�b�s���O���[�h�擾
	FbxLayerElement::EReferenceMode referrenceMode = element->GetReferenceMode();	// ���t�@�����X���[�h�擾
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// �@�������i�[�����������������z��̃C���f�b�N�X�l�z����擾 
	const FbxLayerElementArrayTemplate<FbxVector4>& directArray = element->GetDirectArray();	// �@�������i�[�����������������z����擾

	// eDirect���@eIndexDirect�̂ݑΉ�
	assert((referrenceMode == FbxGeometryElement::eDirect) || (referrenceMode == FbxGeometryElement::eIndexToDirect));

	// �@�����u�d�b�s�n�q
	std::vector<Vector3> normalList;
	normalList.reserve(IndexList_.size());		// ���_�C���f�b�N�X�����m��

	// ���_���W�Ń}�b�s���O����Ă���ꍇ
	if (mappingmode == FbxGeometryElement::eByControlPoint) {
		for (unsigned int i = 0; i < IndexList_.size(); i++) {
			int index = IndexList_[i];		// �ʂ̍\�����z���i�Ԗڂ̒��_�C���f�b�N�X���擾
			int normalIndex;				// �@�����̃C���f�b�N�X
			// ���t�@�����X���[�h�𔻒�
			if (referrenceMode == FbxGeometryElement::eDirect) {		// eDirect�̏ꍇ
				normalIndex = index;								// ���_�C���f�b�N�X�̈ʒu�ɑΉ����ĕۑ�����Ă���
			}
			else {													// eIndexToDirect�̏ꍇ
				normalIndex = indexArray.GetAt(index);				// ���_���W�C���f�b�N�X�ɑΉ������@�����VECTOR�̃C���f�b�N�X���擾
			}
			FbxVector4 normal = directArray.GetAt(normalIndex);		// �@�������擾
			if (normal[3] != 0.0f) {
				if (normal[3] == 1.0f) {
					normalList.push_back(Vector3(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])));			// �������W��
				}
				else {
					normalList.push_back(Vector3(static_cast<float>(normal[0] / normal[3]), static_cast<float>(normal[1] / normal[3]), static_cast<float>(normal[2] / normal[3])));		// �������W��
				}
			}
		}
	}
	else if (mappingmode == FbxGeometryElement::eByPolygonVertex) {
		// ���_�C���f�b�N�X(�ʂ̍\�����)�Ń}�b�s���O����Ă���ꍇ
		int indexByPolygonVertex = 0;
		int polygonCount = Mesh_->GetPolygonCount();					// �|���S�������擾
		for (int i = 0; i < polygonCount; ++i) {
			int polygonSize = Mesh_->GetPolygonSize(i);				// ���_�����擾
			int normalIndex;
			for (int j = 0; j < polygonSize; ++j) {
				// ���t�@�����X���[�h�𔻒�
				if (referrenceMode == FbxGeometryElement::eDirect) {		// eDirect�̏ꍇ
					normalIndex = indexByPolygonVertex;
				}
				else {
					normalIndex = indexArray.GetAt(indexByPolygonVertex);	// eIndexToDirect�̏ꍇ
				}
				FbxVector4 normal = directArray.GetAt(normalIndex);		// �@�������擾
				if (normal[3] != 0.0f) {
					if (normal[3] == 1.0)
					{
						normalList.push_back(Vector3(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])));		// �������W��
					}
					else {
						normalList.push_back(Vector3(static_cast<float>(normal[0] / normal[3]), static_cast<float>(normal[1] / normal[3]), static_cast<float>(normal[2] / normal[3])));	// �������W��
					}
				}
				++indexByPolygonVertex;
			}
		}
	}
	else {
		assert(false);
	}

	return normalList;
}

std::vector<Vector2> CFbxModel::GetUVList(FbxMesh* Mesh_, const std::vector<int>& IndexList_, int UvNo_) {
	std::vector<Vector2> uvList;

	int elementCount = Mesh_->GetElementUVCount();		// ���̂t�u��񂪃Z�b�g����Ă��邩
	if (UvNo_ + 1 > elementCount)						// ����͂P�ɂ̂ݑΉ�
	{
		return uvList;
	}

	FbxGeometryElementUV* element = Mesh_->GetElementUV(UvNo_);					// UV�Z�b�g���擾
	FbxLayerElement::EMappingMode mappingMode = element->GetMappingMode();		// �}�b�s���O���[�h���擾
	FbxLayerElement::EReferenceMode referenceMode = element->GetReferenceMode();	// ���t�@�����X���[�h���擾	
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// �t�u�����i�[�����u�d�b�s�n�q�z��̃C���f�b�N�X�z����擾
	const FbxLayerElementArrayTemplate<FbxVector2>& directArray = element->GetDirectArray();	// �t�u�l�z����擾

	// eDirct��eIndexDirect�̂ݑΉ�
	assert((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

	// ���_�C���f�b�N�X�����������������e�ʂ��m��
	uvList.reserve(IndexList_.size());

	// ���_�ɑΉ����Ċi�[����Ă���ꍇ
	if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		// ���_���W�Ń}�b�s���O
		for (unsigned int i = 0; i < IndexList_.size(); i++)
		{
			int index = IndexList_[i];			// �ʂ̍\�����z�񂩂璸�_�C���f�b�N�X�ԍ����擾

			// ���t�@�����X���[�h�𔻒�
			int uvIndex;
			if (referenceMode == FbxGeometryElement::eDirect) {		// eDirect�̏ꍇ
				uvIndex = index;		//�@eDirect�̏ꍇ�i���_�C���f�b�N�X�Ɠ����C���f�b�N�X�l�ŃZ�b�g����Ă���j
			}
			else {													// eIndexToDirect�̏ꍇ
				uvIndex = indexArray.GetAt(index);				// ���_���W�C���f�b�N�X�ɑΉ������t�u���C���f�b�N�X���擾
			}

			FbxVector2 uv = directArray.GetAt(uvIndex);		// uv�l��double�^�Ŏ擾
			uvList.push_back(Vector2(static_cast<float>(uv[0]), static_cast<float>(uv[1])));		// float�l�Ƃ��Ċi�[
		}
	}
	// �ʂ̍\�����ɑΉ����Ċi�[����Ă���ꍇ
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
	{
		// �|���S���o�[�e�b�N�X�i�ʂ̍\�����̃C���f�b�N�X�j�Ń}�b�s���O
		int indexByPolygonVertex = 0;						// �ʂ̍\�����C���f�b�N�X�z��̃C���f�b�N�X
		int polygonCount = Mesh_->GetPolygonCount();			// ���b�V���̃|���S�������擾
		for (int i = 0; i < polygonCount; ++i)				// �|���S���������[�v
		{
			int polygonSize = Mesh_->GetPolygonSize(i);		// ���Ԗڂ̃|���S�����_�����擾

			// �|���S���̒��_�������[�v
			for (int j = 0; j < polygonSize; ++j)
			{
				// ���t�@�����X���[�h�̔���H
				int uvIndex;
				if (referenceMode == FbxGeometryElement::eDirect) {		// eDirect�̏ꍇ
					uvIndex = indexByPolygonVertex;		//�@eDirect�̏ꍇ�i���_�C���f�b�N�X�Ɠ����C���f�b�N�X�l�ŃZ�b�g����Ă���j
				}
				else {													// eIndexToDirect�̏ꍇ
					uvIndex = indexArray.GetAt(indexByPolygonVertex);	// �ʂ̍\�����C���f�b�N�X�ɑΉ������t�u���C���f�b�N�X���擾
				}
				FbxVector2 uv = directArray.GetAt(uvIndex);

				uvList.push_back(Vector2(static_cast<float>(uv[0]), static_cast<float>(uv[1])));	// �����������l�Ƃ��Ċi�[

				++indexByPolygonVertex;						// ���_�C���f�b�N�X���C���N�������g
			}
		}
	}
	else
	{
		// ����ȊO�̃}�b�s���O���[�h�ɂ͑Ή����Ȃ�
		assert(false);
	}

	return uvList;
}

void CFbxModel::GetWeight(FbxMesh* Mesh_, const std::vector<int>& IndexList_, std::vector<ModelBoneWeight>& BoneWeightList_, std::vector<std::string>& BoneNodeNameList_, std::vector<Matrix4x4>& InvBaseposeMatrixList_) {
	// ���b�V�����̃X�L����񐔂��擾����
	int skinCount = Mesh_->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount == 0)
	{
		return;
	}

	// �X�L����1�ɂ����Ή����Ȃ�
	assert(skinCount <= 1);

	int controlPointsCount = Mesh_->GetControlPointsCount();		// ���b�V���̒��_�����擾
//	using TmpWeight = std::pair<int, float>;					// std::pair *=> 2�̂��̂̓��ꕨ(vs2010�ŃG���[)
																// using TmpWeight��int ��float�̓��ꕨ���s�����v�����������ƒ�`�������ƂɂȂ�
	std::vector<std::vector<std::pair<int, float>>> tmpBoneWeightList(controlPointsCount);	// �{�[���C���f�b�N�X�l�ƃE�F�C�g�l���y�A�ɂ����u�d�b�s�n�q

	// �e�����l����������X�L���������o��
	FbxSkin* skin = static_cast<FbxSkin*>(Mesh_->GetDeformer(0, FbxDeformer::eSkin));
	int clusterCount = skin->GetClusterCount();	// �X�L������N���X�^�i�{�[���j�����擾

	// �N���X�^�[�i�{�[���̎��j�̐������[�v
	for (int i = 0; i < clusterCount; ++i)
	{
		FbxCluster* cluster = skin->GetCluster(i);					// i�Ԗڂ̃N���X�^�i�{�[���j���擾

		// eNormalize�����Ή����Ȃ�
		assert(cluster->GetLinkMode() == FbxCluster::eNormalize);	// ���K������Ă��Ȃ��E�F�C�g�l�͑Ή����Ă��Ȃ�

		BoneNodeNameList_.push_back(cluster->GetLink()->GetName());	// �{�[�������i�[

		int indexCount = cluster->GetControlPointIndicesCount();	// �{�[���ɏ������钸�_�C���f�b�N�X�����擾
		int* indices = cluster->GetControlPointIndices();			// ���_�C���f�b�N�X�z����擾
		double* weights = cluster->GetControlPointWeights();		// �E�F�C�g�l�z����擾

		// �{�[���ɏ������钸�_�C���f�b�N�X�������[�v����
		for (int j = 0; j < indexCount; ++j)
		{
			int controlPointIndex = indices[j];		// ���̃{�[���ɉe�����󂯂�j�Ԗڂ̒��_�C���f�b�N�X�ԍ����擾
			tmpBoneWeightList[controlPointIndex].push_back(std::pair<int, float>(i, static_cast<float>(weights[j])));
			// ���_�C���f�b�N�X�ԍ��̂u�d�b�s�n�q�ʒu�Ɂii�Ԗڂ̃{�[���Ƃ��̃E�G�C�g�l�j
		}

		// �y�[�X�|�[�Y�̋t�s����쐬���Ă���
		Matrix4x4 invBaseposeMatrix;

		FbxMatrix baseposeMatrix = cluster->GetLink()->EvaluateGlobalTransform().Inverse();		// �t�s����v�Z����

		// FbxMatrix->Matrix4x4��
		auto baseposeMatrixPtr = (double*)baseposeMatrix;
		for (int j = 0; j < 16; ++j)
		{
			invBaseposeMatrix.m[j] = static_cast<float>(baseposeMatrixPtr[j]);	// double->float
		}

		InvBaseposeMatrixList_.push_back(invBaseposeMatrix);			// i�Ԗڂ̃{�[���̏����p���������t�s��
	}

	// �R���g���[���|�C���g�ɑΉ������E�F�C�g���쐬
	std::vector<ModelBoneWeight> boneWeightListControlPoints;

	for (unsigned int j = 0; j < tmpBoneWeightList.size(); j++) {
		// ���Ԗڂ̒��_�̃E�F�C�g�l���擾
		std::vector<std::pair<int, float>>& tmpBoneWeight = tmpBoneWeightList[j];

		// �E�F�C�g�l�̑傫���Ń\�[�g
		std::sort(tmpBoneWeight.begin(), tmpBoneWeight.end(),
			[](const std::pair<int, float>& weightA, const std::pair<int, float>& weightB) { return weightA.second > weightB.second; }
		);

		// 1���_��4��葽���E�F�C�g������U���Ă���Ȃ�e�������Ȃ����͖̂�������
		while (tmpBoneWeight.size() > 4)
		{
			tmpBoneWeight.pop_back();
		}

		// 4�ɖ����Ȃ��ꍇ�̓_�~�[��}��
		while (tmpBoneWeight.size() < 4)
		{
			tmpBoneWeight.push_back(std::pair<int, float>(0, 0.0f));
		}

		ModelBoneWeight weight;
		float total = 0.0f;
		for (int i = 0; i < 4; ++i)
		{
			weight.boneIndex[i] = tmpBoneWeight[i].first;
			weight.boneWeight.v[i] = tmpBoneWeight[i].second;

			total += tmpBoneWeight[i].second;
		}

		// �E�F�C�g�̐��K��
		for (int i = 0; i < 4; ++i)
		{
			weight.boneWeight.v[i] /= total;
		}

		boneWeightListControlPoints.push_back(weight);


	}

	// �C���f�b�N�X�œW�J
	for (unsigned int j = 0; j < IndexList_.size(); j++) {
		int index = IndexList_[j];				// j�Ԗڂ̒��_�C���f�b�N�X���擾
		BoneWeightList_.push_back(boneWeightListControlPoints[index]);	// j�Ԗڂ̒��_�̃{�[���ԍ��ƃE�G�C�g�l���Z�b�g
	}
}

bool CFbxModel::CreateVertexBuffer(unsigned int Vertexnum_, void* Vertexdata_, ID3D11Buffer** pVertexBuffer_) {
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * Vertexnum_;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = Vertexdata_;
	HRESULT hr = GetDX11Device()->CreateBuffer(&bd, &InitData, pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	return true;
}

bool CFbxModel::CreateIndexBuffer(unsigned int Indexnum_, void* Indexdata_, ID3D11Buffer** pIndexBuffer_) {

	// �C���f�b�N�X�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * Indexnum_;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = Indexdata_;
	HRESULT hr = GetDX11Device()->CreateBuffer(&bd, &InitData, pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateBuffer(index buffer) error", "Error", MB_OK);
		return false;
	}

	return true;
}
bool CFbxModel::createConstantBuffer(unsigned int Bytesize_, ID3D11Buffer** pConstantBuffer_) {
	// �R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = Bytesize_;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HRESULT hr = GetDX11Device()->CreateBuffer(&bd, nullptr, pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateBuffer(constant buffer) error", "Error", MB_OK);
		return false;
	}

	return true;
}

void CFbxModel::MakeWorldMatrix(XMMATRIX& mat_, XMFLOAT3& Angle_, XMFLOAT3& Trans_) {

	XMMATRIX matx, maty, matz, mattrans;
	matx = XMMatrixRotationX((Angle_.x*XM_PI) / 180.0f);
	maty = XMMatrixRotationY((Angle_.y*XM_PI) / 180.0f);
	matz = XMMatrixRotationZ((Angle_.z*XM_PI) / 180.0f);
	mattrans = XMMatrixTranslation(Trans_.x, Trans_.y, Trans_.z);

	mat_ = matx * maty*matz*mattrans;
}

void CFbxModel::Draw() {

	//�ړ�----

	//--------
	XMFLOAT3 angle = { 0,0,0 };
	XMFLOAT3 trans = { 0,0,100 };
	MakeWorldMatrix(g_World, angle, trans);

	//
	// �R���X�^���g�o�b�t�@���̍s����X�V
	//
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);					// �]�u���Đݒ�i���[���h�ϊ��s��j
	cb.mView = XMMatrixTranspose(g_View);						// �]�u���Đݒ�i�J�����ϊ��s��j
	cb.mProjection = XMMatrixTranspose(g_Projection);			// �]�u���Đݒ�i�v���W�F�N�V�����ϊ��s��j
	cb.mLightDirection = XMFLOAT3(1, 1, 1);						// ���̕����x�N�g���Z�b�g	
	cb.mCameraPos.x = 0.0f;										// �J�����̈ʒu
	cb.mCameraPos.y = 0.0f;
	cb.mCameraPos.z = -1500.0f;
	GetDX11DeviceContext()->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);	// �X�V

	// Z�o�b�t�@�N���A
	GetDX11DeviceContext()->ClearDepthStencilView(GetDX11Obj()->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// �V�F�[�_�[�Z�b�g
	GetDX11DeviceContext()->VSSetShader(m_pVertexShader, nullptr, 0);			// ���_�V�F�[�_�[���Z�b�g
	GetDX11DeviceContext()->PSSetShader(m_pPixelShader, nullptr, 0);			// �s�N�Z���V�F�[�_�[���Z�b�g

	GetDX11DeviceContext()->IASetInputLayout(m_pVertexLayout);

	// �R���X�^���g�o�b�t�@���Z�b�g�i�j
	GetDX11DeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);	// �R���X�^���g�o�b�t�@���Z�b�g

	// �T���v���[�̃Z�b�g
	GetDX11DeviceContext()->PSSetSamplers(0, 1, &m_pSampleLinear);				// �T���v���[�̃Z�b�g

	// �v���~�e�B�u���Z�b�g
	GetDX11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// UNITY�����(�����̃��b�V���̏W���̂Ƃ��ďo���オ���Ă���)
	for (unsigned int i = 0; i < m_MeshList.size(); i++) {

		unsigned int stride = sizeof(SimpleVertex);
		unsigned  offset = 0;

		// �����X�L���̃��b�V���łȂ��ꍇ
		if (m_MeshList[i].skincount == 0)
		{
			getMeshmatrix(m_Frame,					// �t���[����	
				i,									// ���b�V���h�c
				m_BoneMatrixList[0]);

			ConstantBufferBoneMatrix cb;
			for (int k = 0; k < 1; k++)
			{
				cb.mBoneMatrix[k] = XMMATRIX(
					m_BoneMatrixList[k].m[0], m_BoneMatrixList[k].m[1], m_BoneMatrixList[k].m[2], m_BoneMatrixList[k].m[3],
					m_BoneMatrixList[k].m[4], m_BoneMatrixList[k].m[5], m_BoneMatrixList[k].m[6], m_BoneMatrixList[k].m[7],
					m_BoneMatrixList[k].m[8], m_BoneMatrixList[k].m[9], m_BoneMatrixList[k].m[10], m_BoneMatrixList[k].m[11],
					m_BoneMatrixList[k].m[12], m_BoneMatrixList[k].m[13], m_BoneMatrixList[k].m[14], m_BoneMatrixList[k].m[15]);

				cb.mBoneMatrix[k] = XMMatrixTranspose(cb.mBoneMatrix[k]);		// �]�u���Ȃ��Ƃ�������
			}
			GetDX11DeviceContext()->UpdateSubresource(m_pConstantBufferBoneMatrix, 0, nullptr, &cb, 0, 0);	// �X�V

			// �R���X�^���g�o�b�t�@���Z�b�g(�{�[���s��)
			GetDX11DeviceContext()->VSSetConstantBuffers(1, 1, &m_pConstantBufferBoneMatrix);	// �R���X�^���g�o�b�t�@���Z�b�g�i�{�[���s��j


			// �}�e���A�������擾
			std::string materialname = m_MeshList[i].materialName;

			// ���̃}�e���A���������}�e���A����������
			ModelMaterial material;
			material = GetMaterial(materialname);

			//
			// �R���X�^���g�o�b�t�@���̃}�e���A�����X�V
			//
			ConstantBufferMaterial cbm;
			cbm.diffuse.x = material.Diffuse.v[0];
			cbm.diffuse.y = material.Diffuse.v[1];
			cbm.diffuse.z = material.Diffuse.v[2];
			cbm.diffuse.w = material.Diffuse.v[3];

			cbm.specular.x = material.Speqular.v[0];
			cbm.specular.y = material.Speqular.v[1];
			cbm.specular.z = material.Speqular.v[2];
			cbm.specular.w = material.Speqular.v[3];

			cbm.ambient.x = material.Ambient.v[0];
			cbm.ambient.y = material.Ambient.v[1];
			cbm.ambient.z = material.Ambient.v[2];
			cbm.ambient.w = material.Ambient.v[3];

			cbm.power.x = material.Power;
			cbm.power.y = material.Power;
			cbm.power.z = material.Power;
			cbm.power.w = material.Power;

			GetDX11DeviceContext()->UpdateSubresource(m_pConstantBufferMaterial, 0, nullptr, &cbm, 0, 0);	// �X�V

			// �R���X�^���g�o�b�t�@���Z�b�g(�{�[���s��)
			GetDX11DeviceContext()->VSSetConstantBuffers(2, 1, &m_pConstantBufferMaterial);	// �R���X�^���g�o�b�t�@���Z�b�g�i�}�e���A���j

			// ���_�o�b�t�@�Z�b�g
			GetDX11DeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer[i], &stride, &offset);

			// �C���f�b�N�X�o�b�t�@���Z�b�g
			GetDX11DeviceContext()->IASetIndexBuffer(m_pIndexBuffer[i], DXGI_FORMAT_R16_UINT, 0);

			// �e�N�X�`�����Z�b�g
			GetDX11DeviceContext()->PSSetShaderResources(0, 1, &m_Texture[i]);			// �e�N�X�`�����Z�b�g

			// �`��
			GetDX11DeviceContext()->DrawIndexed(m_MeshList[i].indexList.size(), 0, 0);
		}
		// �����X�L���̃��b�V���̏ꍇ
		else
		{
			getBoneMatrix(m_Frame, i, m_BoneMatrixList, 400);
			ConstantBufferBoneMatrix cb;
			for (int k = 0; k < 400; k++)
			{
				cb.mBoneMatrix[k] = XMMATRIX(
					m_BoneMatrixList[k].m[0], m_BoneMatrixList[k].m[1], m_BoneMatrixList[k].m[2], m_BoneMatrixList[k].m[3],
					m_BoneMatrixList[k].m[4], m_BoneMatrixList[k].m[5], m_BoneMatrixList[k].m[6], m_BoneMatrixList[k].m[7],
					m_BoneMatrixList[k].m[8], m_BoneMatrixList[k].m[9], m_BoneMatrixList[k].m[10], m_BoneMatrixList[k].m[11],
					m_BoneMatrixList[k].m[12], m_BoneMatrixList[k].m[13], m_BoneMatrixList[k].m[14], m_BoneMatrixList[k].m[15]);

				cb.mBoneMatrix[k] = XMMatrixTranspose(cb.mBoneMatrix[k]);		// �]�u���Ȃ��Ƃ�������
			}
			GetDX11DeviceContext()->UpdateSubresource(m_pConstantBufferBoneMatrix, 0, nullptr, &cb, 0, 0);	// �X�V

			// �R���X�^���g�o�b�t�@���Z�b�g(�{�[���s��)
			GetDX11DeviceContext()->VSSetConstantBuffers(1, 1, &m_pConstantBufferBoneMatrix);	// �R���X�^���g�o�b�t�@���Z�b�g�i�{�[���s��j

			// �}�e���A�������擾
			std::string materialname = m_MeshList[i].materialName;
			ConstantBufferMaterial cbm;
			if (materialname != "noName")
			{

				// ���̃}�e���A���������}�e���A����������
				static ModelMaterial material;
				material = GetMaterial(materialname);

				//
				// �R���X�^���g�o�b�t�@���̃}�e���A�����X�V
				//

				cbm.diffuse.x = material.Diffuse.v[0];
				cbm.diffuse.y = material.Diffuse.v[1];
				cbm.diffuse.z = material.Diffuse.v[2];
				cbm.diffuse.w = material.Diffuse.v[3];

				cbm.specular.x = material.Speqular.v[0];
				cbm.specular.y = material.Speqular.v[1];
				cbm.specular.z = material.Speqular.v[2];
				cbm.specular.w = material.Speqular.v[3];

				cbm.ambient.x = material.Ambient.v[0];
				cbm.ambient.y = material.Ambient.v[1];
				cbm.ambient.z = material.Ambient.v[2];
				cbm.ambient.w = material.Ambient.v[3];

				cbm.power.x = material.Power;
				cbm.power.y = material.Power;
				cbm.power.z = material.Power;
				cbm.power.w = material.Power;
			}

			GetDX11DeviceContext()->UpdateSubresource(m_pConstantBufferMaterial, 0, nullptr, &cbm, 0, 0);	// �X�V

			// �R���X�^���g�o�b�t�@���Z�b�g�i�}�e���A���j
			GetDX11DeviceContext()->VSSetConstantBuffers(2, 1, &m_pConstantBufferMaterial);

			// ���_�o�b�t�@�Z�b�g
			GetDX11DeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer[i], &stride, &offset);

			// �C���f�b�N�X�o�b�t�@���Z�b�g
			GetDX11DeviceContext()->IASetIndexBuffer(m_pIndexBuffer[i], DXGI_FORMAT_R16_UINT, 0);

			// �e�N�X�`�����Z�b�g
			GetDX11DeviceContext()->PSSetShaderResources(0, 1, &m_Texture[i]);			// �e�N�X�`�����Z�b�g

			// �`��
			GetDX11DeviceContext()->DrawIndexed(m_MeshList[i].indexList.size(), 0, 0);
		}
	}

	m_Frame++;

	// �A�j���[�V�����I��������ŏ�����
	if (isAnimationEnd(m_Frame))
	{
		m_Frame = 0;
	}
}