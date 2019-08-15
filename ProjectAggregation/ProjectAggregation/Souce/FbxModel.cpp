
#include "Shader.h"
#include "DX11util.h"
#include "Utility.h"
#include "FbxModel.h"

//単位行列
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
	//Fbxモデル読み込み

	//頂点シェーダーをコンパイル
	//ID3DBlob* pVSBlob = nullptr;
	bool hr/* = CompileShaderFromFile("Shader/fbxtest.fx", "main", "vs_4_0", &pVSBlob)*/;

	//if (FAILED(hr)) {
	//	MessageBox(nullptr,
	//		"FX file compiled error.", "Error", MB_OK);

	//}

	//頂点レイアウト設定
	D3D11_INPUT_ELEMENT_DESC layout[] = {

		{ "POSITION",        0,      DXGI_FORMAT_R32G32B32_FLOAT,      0,      0,										D3D11_INPUT_PER_VERTEX_DATA, 0 },	// 頂点座標
		{ "COLOR",           0,      DXGI_FORMAT_R32G32B32A32_FLOAT,   0,      4 * 3,									D3D11_INPUT_PER_VERTEX_DATA, 0 },	// 頂点カラー
		{ "NORMAL",          0,      DXGI_FORMAT_R32G32B32_FLOAT,      0,      4 * 4 + 4 * 3 ,							D3D11_INPUT_PER_VERTEX_DATA, 0 },	// 法線ベクトル
		{ "TEXCOORD",        0,      DXGI_FORMAT_R32G32_FLOAT,         0,      4 * 3 + 4 * 4 + 4 * 3,					D3D11_INPUT_PER_VERTEX_DATA, 0 },	// テクスチャ座標
		{ "BONEINDEX",		 0,		 DXGI_FORMAT_R32G32B32A32_FLOAT,   0,	   4 * 2 + 4 * 3 + 4 * 4 + 4 * 3,			D3D11_INPUT_PER_VERTEX_DATA, 0 },	// ボーン行列インデクッス
		{ "BONEWEIGHT",		 0,		 DXGI_FORMAT_R32G32B32A32_FLOAT,   0,	   4 * 4 + 4 * 2 + 4 * 3 + 4 * 4 + 4 * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },	// ボーンウェイト
	};
	unsigned int numElements = ARRAYSIZE(layout);
	//頂点シェーダー作成

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
	//頂点データをデバイスにセット
	GetDX11DeviceContext()->IASetInputLayout(m_pVertexLayout);


	//ピクセルシェーダー作成
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


	////頂点シェーダー作成
	//hr = GetDX11Device()->CreateVertexShader(
	//	pVSBlob->GetBufferPointer(),
	//	pVSBlob->GetBufferSize(),
	//	nullptr,
	//	&m_pVertexShader
	//);
	//if (FAILED(hr)) {
	//	pVSBlob->Release();
	//}
	////頂点レイアウト生成
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

	////解放
	//pVSBlob->Release();


	////ピクセルシェーダーをコンパイル
	//ID3DBlob* pPSBlob = nullptr;
	//hr = CompileShaderFromFile("Shader/fbxtest.fx", "PS", "ps_4_0", &pPSBlob);
	//if (FAILED(hr)) {
	//	MessageBox(nullptr, "Pixel Shader Error", "error", MB_OK);

	//}

	////ピクセルシェーダ生成
	//hr = GetDX11Device()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	//if (FAILED(hr)) {
	//	MessageBox(nullptr, "Create Pixelshader Error", "error", MB_OK);

	//}
	//pPSBlob->Release();

	//モデル読み込み
	LoadModel(ModelName_);

	//頂点バッファ生成
	for (unsigned int i = 0; i < m_MeshList.size(); i++) {
		SimpleVertex* v;
		v = new SimpleVertex[m_MeshList[i].vertexList.size()];

		for (unsigned int j = 0; j < m_MeshList[i].vertexList.size(); j++) {

			//頂点座標をセット
			v[j].Pos.x = m_MeshList[i].vertexList[j].position.x;
			v[j].Pos.y = m_MeshList[i].vertexList[j].position.y;
			v[j].Pos.z = m_MeshList[i].vertexList[j].position.z;

			//頂点カラーをセット
			v[j].Color.w = 1.0f;
			v[j].Color.x = 1.0f;
			v[j].Color.y = 1.0f;
			v[j].Color.z = 1.0f;

			//法線ベクトルをセット
			v[j].Normal.x = m_MeshList[i].vertexList[j].normal.x;
			v[j].Normal.y = m_MeshList[i].vertexList[j].normal.y;
			v[j].Normal.z = m_MeshList[i].vertexList[j].normal.z;

			//テクスチャ座標をセット
			v[j].Tex.x = m_MeshList[i].vertexList[j].uv0.x;
			v[j].Tex.y = 1.0f - m_MeshList[i].vertexList[j].uv0.y;

			//ボーン行列インデックスをセット
			v[j].BoneIndex.w = m_MeshList[i].vertexList[j].boneIndex[0];
			v[j].BoneIndex.x = m_MeshList[i].vertexList[j].boneIndex[1];
			v[j].BoneIndex.y = m_MeshList[i].vertexList[j].boneIndex[2];
			v[j].BoneIndex.z = m_MeshList[i].vertexList[j].boneIndex[3];

			//ボーンウェイト値をセット
			v[j].BoneWeighs.w = m_MeshList[i].vertexList[j].boneWeight.v[0];
			v[j].BoneWeighs.x = m_MeshList[i].vertexList[j].boneWeight.v[1];
			v[j].BoneWeighs.y = m_MeshList[i].vertexList[j].boneWeight.v[2];
			v[j].BoneWeighs.z = m_MeshList[i].vertexList[j].boneWeight.v[3];
		}

		//頂点バッファ生成
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

	//インデックスバッファ作成
	for (unsigned int i = 0; i < m_MeshList.size(); i++) {
		WORD* indexList;

		//インデックス数取得
		indexList = new WORD[m_MeshList[i].indexList.size()];

		//インデックスデータセット
		for (unsigned int j = 0; j < m_MeshList[i].indexList.size(); j++) {
			indexList[j] = m_MeshList[i].indexList[j];
		}

		//インデックスバッファ作成
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

	//シェーダーリソースビュー生成
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

		//テクスチャ読み込み
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

		//シェーダーリソースビュー作成
		hr = DirectX::CreateShaderResourceView(GetDX11Device(), image->GetImages(), image->GetImageCount(), meta, &m_Texture[i]);
		if (FAILED(hr)) {
			MessageBox(nullptr, "Create ShaderResouceView Error", "error", MB_OK);
		}
	}

	//コンスタントバッファ作成
	sts = createConstantBuffer(
		sizeof(ConstantBuffer),
		&m_pConstantBuffer
	);
	if (!sts) {
		MessageBox(nullptr, "Create Constant Buffer Error", "error", MB_OK);
	}

	//ボーン行列用コンスタントバッファ作成
	sts = createConstantBuffer(
		sizeof(ConstantBuffer),
		&m_pConstantBufferBoneMatrix
	);
	if (!sts) {
		MessageBox(nullptr, "Create Constant Buffer Error", "error", MB_OK);
	}

	//ボーンマテリアル用コンスタントバッファ作成
	sts = createConstantBuffer(
		sizeof(ConstantBuffer),
		&m_pConstantBufferMaterial
	);
	if (!sts) {
		MessageBox(nullptr, "Create Constant Buffer Error", "error", MB_OK);
	}

	//ワールド変換行列初期化
	g_World = XMMatrixIdentity();

	XMVECTOR Eye = XMVectorSet(0.0f, 100.0f, -100.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	// プロジェクション変換行列初期化
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800 / (FLOAT)600, 0.01f, 10000.0f);

	// テクスチャ用サンプラー作成
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

	//FbxManager生成
	g_pFbxManager = FbxManager::Create();
	if (!g_pFbxManager) {
		//error
		exit(1);
	}

	FbxIOSettings* ios = FbxIOSettings::Create(g_pFbxManager, IOSROOT);
	g_pFbxManager->SetIOSettings(ios);

	//インポーター作成
	m_Importer = FbxImporter::Create(g_pFbxManager, "");
	if (!m_Importer->Initialize(Modelname_, -1, g_pFbxManager->GetIOSettings())) {
		exit(1);
	}
	//シーン作成
	m_Scene = FbxScene::Create(g_pFbxManager, "Scene");
	if (!m_Scene) {
		//error
		exit(1);
	}

	//Fbxをパースしてシーンに読み込む
	bool sts = m_Importer->Import(m_Scene);
	if (!sts) {
		exit(1);
	}

	//インポーター解放
	m_Importer->Destroy();

	//ポリゴンを三角形化
	FbxGeometryConverter geometryConverter(g_pFbxManager);
	geometryConverter.Triangulate(m_Scene, true);

	//メッシュの解析
	int MeshCount = m_Scene->GetMemberCount<FbxMesh>();
	printf("メッシュの数 %d \n", MeshCount);

	int NodeCount = m_Scene->GetNodeCount();
	printf("ノードの数 %d \n", NodeCount);

	//ノード名を辞書に登録
	for (int i = 0; i < NodeCount; ++i) {
		FbxNode* fbxNode = m_Scene->GetNode(i);
		m_NodeIdDictionary.insert(std::pair<std::string, int>(fbxNode->GetName(), i));
	}

	//ノードの中身を確認
	for (std::map<std::string, int>::iterator it = m_NodeIdDictionary.begin(); it != m_NodeIdDictionary.end(); ++it) {
		//		std::cout << "key = " << (*it).first << ",value = " << (*it).second << std::endl;
	}

	//シーンに含まれるマテリアルの解析
	static int materialCount = m_Scene->GetMaterialCount();
	
	m_MaterialList.reserve(materialCount);
	for (int i = 0; i < materialCount; i++) {
		FbxSurfaceMaterial* fbxMaterial = m_Scene->GetMaterial(i);

		ModelMaterial modelMaterial = ParseMaterial(fbxMaterial);
		m_MaterialList.push_back(modelMaterial);
		m_MaterialIdDictionary.insert(std::pair<std::string, int>(modelMaterial.materialName,i));
	}

	//シーンに含まれるメッシュの解析
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

	if (modelMesh.boneNodeNameList.size() == 0) {//ボーン名リスト
		OutMatrixList[0] = Matrix4x4Identity;
		return;
	}

	assert(modelMesh.boneNodeNameList.size() <= MatrixCount_);//ボーン数が配列数より多い場合終了

	FbxTime time;
	time.Set(FbxTime::GetOneFrameValue(FbxTime::eFrames60) * Frame_); //経過時間に合わせてフレームセット

	//このメッシュに含まれているボーン数分ループ
	for (unsigned int i = 0; i < modelMesh.boneNodeNameList.size(); i++) {
		std::string& boneNodeName = modelMesh.boneNodeNameList[i];
		int boneNodeId = m_NodeIdDictionaryAnimation.at(boneNodeName);
		FbxNode* boneNode = m_FbxAnimationScene->GetNode(boneNodeId);

		FbxMatrix boneMatrix = boneNode->EvaluateGlobalTransform(time);		// 経過時間に応じた姿勢をあらわすボーン行列を取得
		Matrix4x4& out_matrix = OutMatrixList[i];

		// FbxMatrix（doubleの配列）からMatrix4x4(floatの配列)へ変換
		double* boneMatrixPtr = (double*)boneMatrix;
		for (int j = 0; j < 16; ++j)
		{
			out_matrix.m[j] = static_cast<float>(boneMatrixPtr[j]);			// double->float
		}

		MulMatrixMatrix(modelMesh.invBoneBaseposeMatrixList[i], out_matrix, out_matrix);		// ボーンオフセット行列＊ボーン行列

	}
}

void CFbxModel::getMeshmatrix(INT64 Frame_, int MeshId_, Matrix4x4& OutMatrix_) {
	ModelMesh& modelMesh = m_MeshList[MeshId_];

	std::map<std::string, int>::iterator it = m_NodeIdDictionaryAnimation.find(modelMesh.nodeName);		// メッシュのノード名からノードＩＤを取得
	// 見つからなかった場合
	if (it == m_NodeIdDictionaryAnimation.end())
	{
		OutMatrix_ = Matrix4x4Identity;
		return;
	}

	int meshNodeId = it->second;														// ノードＩＤ取得
	FbxNode* meshNode = m_FbxAnimationScene->GetNode(meshNodeId);				// FBXノードを取得

	FbxTime time;
	time.Set(FbxTime::GetOneFrameValue(FbxTime::eFrames60) * Frame_);		// 指定フレームに応じた経過時間をセット

	FbxMatrix meshMatrix = meshNode->EvaluateGlobalTransform(time);			// 経過時間に応じた姿勢をあらわす行列を取得

	// FbxMatrix（doubleの配列）からMatrix4x4(floatの配列)へ変換
	double* meshMatrixPtr = (double*)meshMatrix;
	for (int i = 0; i < 16; ++i)
	{
		OutMatrix_.m[i] = static_cast<float>(meshMatrixPtr[i]);				// double->float
	}

	MulMatrixMatrix(modelMesh.invMeshBaseposeMatrix, OutMatrix_, OutMatrix_);		// オフセット行列＊メッシュ姿勢行列

}

void CFbxModel::LoadFbxAnimation(const char* AnimationFileName_) {
	// すでにアニメーションを読み込んでいたら
	if (m_FbxAnimationScene != nullptr)
	{
		m_FbxAnimationScene->Destroy();
		m_FbxAnimationScene = nullptr;
		m_NodeIdDictionaryAnimation.clear();
	}

	//アニメーション情報取得
	//Importer生成
	m_Importer = FbxImporter::Create(g_pFbxManager, "");

	if (!m_Importer->Initialize(AnimationFileName_, -1, g_pFbxManager->GetIOSettings()))
	{
		//		printf("Error: Unable to create FBX m_Importer(animation)!\n");
		exit(1);
	}

	//animationScene生成
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

	//FBXをパースしてm_FbxAnimationSceneに読み込む
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

	//アニメーション数取得
	int animStackCount = m_Importer->GetAnimStackCount();				// アニメーション数を取得
	//assert(animStackCount == 1);									// １アニメーションだけに対応
	FbxTakeInfo* takeInfo = m_Importer->GetTakeInfo(0);				// ０番目のＴＡＫＥを取得

	FbxTime importOffset = takeInfo->mImportOffset;					// オフセット時間
	FbxTime startTime = takeInfo->mLocalTimeSpan.GetStart();		// 開始時刻を取得
	FbxTime stopTime = takeInfo->mLocalTimeSpan.GetStop();			// 終了時刻を取得

	// アニメーション開始時間時間／１フレームの時間で開始フレームを求めている
	m_AnimationFrame_Start = (importOffset.Get() + startTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::eFrames60);
	// アニメーション終了時間／１フレームの時間の時間で終了フレームを求めている
	m_AnimationFrame_End = (importOffset.Get() + stopTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::eFrames60);
	m_Importer->Destroy();

	// ノード名からノードIDを取得できるように辞書に登録
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
	// マテリアル名でインデックスをサーチ
	std::map<std::string, int>::iterator it = m_MaterialIdDictionary.find(MaterialName_);
	return m_MaterialList[(*it).second];
}

bool CFbxModel::isAnimationEnd(INT64 Frame_) {
	///
	return false;
}

ModelMesh CFbxModel::ParseMesh(FbxMesh* Mesh_) {
	assert(Mesh_ != nullptr);

	FbxNode* fbxnode = Mesh_->GetNode();					// ＦＢＸノードを取得

	// マテリアルが1のものしか対応しない

	int materialCount = fbxnode->GetMaterialCount();
	//assert(fbxnode->GetMaterialCount() == 1);

	ModelMesh modelMesh;								// 最終的に保存されるメッシュ情報（この関数の出力値）

	// ワンスキンの数を取得
	modelMesh.skincount = Mesh_->GetDeformerCount(FbxDeformer::eSkin);		// ワンスキン情報の数をセット
	modelMesh.nodeName = fbxnode->GetName();	// ＦＢＸノード名をセット
	if (materialCount > 0)
	{
		modelMesh.materialName = fbxnode->GetMaterial(0)->GetName();			// マテリアル名をセット
	}
	else
	{
		modelMesh.materialName = "noName";
	}
	printf("Mesh_:%s ,%s \n", modelMesh.nodeName.c_str(), modelMesh.materialName.c_str());

	// ベースポーズの逆行列を作成しておく(メッシュの)
	FbxMatrix baseposeMatrix = fbxnode->EvaluateGlobalTransform().Inverse();

	double* baseposeMatrixPtr = (double*)baseposeMatrix;

	for (int i = 0; i < 16; ++i)
	{
		modelMesh.invMeshBaseposeMatrix.m[i] = static_cast<float>(baseposeMatrixPtr[i]);
	}

	// インデックスＶＥＣＴＯＲ取得
	std::vector<int> indexList = GetIndexList(Mesh_);

	// 頂点データ取得
	std::vector<Vector3> positionList = GetPositionList(Mesh_, indexList);		// 頂点座標を取得（面の構成情報に合わせる）
	std::vector<Vector3> normalList = GetNormalList(Mesh_, indexList);			// 法線ベクトルを取得（面の構成情報に合わせる）
	std::vector<Vector2> uv0List = GetUVList(Mesh_, indexList, 0);				// UV座標を取得（面の構成情報に合わせる）

	// ウエイト値を取得する
	std::vector<ModelBoneWeight> boneWeightList;
	GetWeight(Mesh_,											// メッシュ（入力）
		indexList,										// 面の構成情報（入力）
		boneWeightList,									// ボーン情報リスト（出力値）（ボーン情報へのインデックス値とウェイト値）
		modelMesh.boneNodeNameList,						// メッシュ内のボーン名をＶＥＣＴＯＲに保存（出力）
		modelMesh.invBoneBaseposeMatrixList);			// ボーンオフセット行列をＶＥＣＴＯＲに保存（）

// テンポラリの頂点情報（ＭｏｄｅｌＶｅｒｔｅｘ）を作る
	std::vector<ModelVertex> tmpmodelVertexList;
	tmpmodelVertexList.reserve(indexList.size());		// 頂点インデックスサイズ分容量を確保

	// 頂点インデックス数分（面の構成情報順）ループする
	for (unsigned int i = 0; i < indexList.size(); ++i)
	{
		ModelVertex vertex;
		vertex.position = positionList[i];			// 頂点情報

		if (normalList.size() != 0)
		{
			vertex.normal = normalList[i];				// 法線情報
		}

		if (uv0List.size() == 0)
		{
			vertex.uv0 = Vector2(0.0f, 0.0f);
		}
		else
		{
			vertex.uv0 = uv0List[i];
		}

		// ボーンウェイト値が存在するか？
		if (boneWeightList.size() > 0)
		{
			// ボーンのインデックス番号をセット
			for (int j = 0; j < 4; ++j)
			{
				vertex.boneIndex[j] = boneWeightList[i].boneIndex[j];
			}
			// ウェイト値をセット
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

	// 重複頂点を除いてインデックス作成
	modelMesh.vertexList.reserve(tmpmodelVertexList.size());		// 頂点情報数分エリアを確保する

	modelMesh.indexList.reserve(indexList.size());		// 面の構成情報数分エリアを確保する

	// ここで重複している頂点データを除き、頂点データリストとインデックスリストを作り直している
	// 重複した頂点を含んだ頂点数分ループしている(面の構成情報を元に抜き出した頂点情報)
	for (unsigned int i = 0; i < tmpmodelVertexList.size(); i++)
	{
		ModelVertex& vertex = tmpmodelVertexList[i];
		// 重複しているか？
		// modelMesh.vertexListは、最初空でだんだん登録されていく（重複していない頂点情報として）
		auto it = std::find(modelMesh.vertexList.begin(), modelMesh.vertexList.end(), vertex);
		if (it == modelMesh.vertexList.end())
		{
			// 重複していない
			modelMesh.indexList.push_back(modelMesh.vertexList.size());	// 頂点インデックスを格納
			modelMesh.vertexList.push_back(vertex);					// 頂点情報を格納
		}
		else
		{
			// 重複している
			auto index = std::distance(modelMesh.vertexList.begin(), it);	// 先頭から現イテレータ（重複頂点した先頭データを指し示している）までのインデックス番号を取得
			modelMesh.indexList.push_back(index);// インデックス番号（重複頂点した先頭データを指し示している）をインデックスリストにセット
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

	// マテリアル情報取得
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

	// テクスチャを取得
	auto implementation = GetImplementation(Material_, FBXSDK_IMPLEMENTATION_CGFX);
	//assert(implementation != nullptr);

	//取得できなかったら真っ黒モデルを描画する
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
	int polygonCount = Mesh_->GetPolygonCount();			// 三角形の数を取得
	std::vector<int> indexList;							// 頂点インデックス（面の構成情報）
	indexList.reserve(polygonCount * 3);				// 3角形の数×３

	// 面の構成情報を取得する
	for (int i = 0; i < polygonCount; i++)
	{
		indexList.push_back(Mesh_->GetPolygonVertex(i, 0));		// i番目の三角形の０番目の頂点インデックスを取得
		indexList.push_back(Mesh_->GetPolygonVertex(i, 1));		// i番目の三角形の１番目の頂点インデックスを取得
		indexList.push_back(Mesh_->GetPolygonVertex(i, 2));		// i番目の三角形の２番目の頂点インデックスを取得
	}
	return indexList;
}

std::vector<Vector3> CFbxModel::GetPositionList(FbxMesh* Mesh_, const std::vector<int>& IndexList_) {
	//　頂点座標を取得
	std::vector<Vector3> positionList;					//	頂点座標格納ＶＥＣＴＯＲ 
	positionList.reserve(IndexList_.size());				//  

	for (unsigned int i = 0; i < IndexList_.size(); i++) {
		int index = IndexList_[i];						// i番目の頂点情報（面の構成上情報順に並んでいる）を取得
		FbxVector4 Position = Mesh_->GetControlPointAt(index);		// index番目の頂点座標を取得
		if (Position[3] != 0.0) {
			if (Position[3] == 1.0) {
				positionList.push_back(Vector3(static_cast<float>(Position[0]), static_cast<float>(Position[1]), static_cast<float>(Position[2])));			// 同次座標化
			}
			else {
				positionList.push_back(Vector3(static_cast<float>(Position[0] / Position[3]), static_cast<float>(Position[1] / Position[3]), static_cast<float>(Position[2] / Position[3])));		// 同次座標化
			}
		}
		else {
			positionList.push_back(Vector3(static_cast<float>(Position[0]), static_cast<float>(Position[1]), static_cast<float>(Position[2])));				// Ｗを無視して格納
		}
	}
	return positionList;
}

std::vector<Vector3> CFbxModel::GetNormalList(FbxMesh* Mesh_, const std::vector<int>& IndexList_) {
	int elementCount = Mesh_->GetElementNormalCount();			// 何個の法線情報がセットされているか

	assert(elementCount == 1);					// １個の法線情報にしか対応しない

	FbxGeometryElementNormal* element = Mesh_->GetElementNormal(0);	// ０番目の法線セットを取得
	FbxLayerElement::EMappingMode mappingmode = element->GetMappingMode();	// マッピングモード取得
	FbxLayerElement::EReferenceMode referrenceMode = element->GetReferenceMode();	// リファレンスモード取得
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// 法線情報を格納したｖｅｃｔｏｒ配列のインデックス値配列を取得 
	const FbxLayerElementArrayTemplate<FbxVector4>& directArray = element->GetDirectArray();	// 法線情報を格納したｖｅｃｔｏｒ配列を取得

	// eDirectか　eIndexDirectのみ対応
	assert((referrenceMode == FbxGeometryElement::eDirect) || (referrenceMode == FbxGeometryElement::eIndexToDirect));

	// 法線情報ＶＥＣＴＯＲ
	std::vector<Vector3> normalList;
	normalList.reserve(IndexList_.size());		// 頂点インデックス数分確保

	// 頂点座標でマッピングされている場合
	if (mappingmode == FbxGeometryElement::eByControlPoint) {
		for (unsigned int i = 0; i < IndexList_.size(); i++) {
			int index = IndexList_[i];		// 面の構成情報配列のi番目の頂点インデックスを取得
			int normalIndex;				// 法線情報のインデックス
			// リファレンスモードを判定
			if (referrenceMode == FbxGeometryElement::eDirect) {		// eDirectの場合
				normalIndex = index;								// 頂点インデックスの位置に対応して保存されている
			}
			else {													// eIndexToDirectの場合
				normalIndex = indexArray.GetAt(index);				// 頂点座標インデックスに対応した法線情報VECTORのインデックスを取得
			}
			FbxVector4 normal = directArray.GetAt(normalIndex);		// 法線情報を取得
			if (normal[3] != 0.0f) {
				if (normal[3] == 1.0f) {
					normalList.push_back(Vector3(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])));			// 同次座標へ
				}
				else {
					normalList.push_back(Vector3(static_cast<float>(normal[0] / normal[3]), static_cast<float>(normal[1] / normal[3]), static_cast<float>(normal[2] / normal[3])));		// 同次座標へ
				}
			}
		}
	}
	else if (mappingmode == FbxGeometryElement::eByPolygonVertex) {
		// 頂点インデックス(面の構成情報)でマッピングされている場合
		int indexByPolygonVertex = 0;
		int polygonCount = Mesh_->GetPolygonCount();					// ポリゴン数を取得
		for (int i = 0; i < polygonCount; ++i) {
			int polygonSize = Mesh_->GetPolygonSize(i);				// 頂点数を取得
			int normalIndex;
			for (int j = 0; j < polygonSize; ++j) {
				// リファレンスモードを判定
				if (referrenceMode == FbxGeometryElement::eDirect) {		// eDirectの場合
					normalIndex = indexByPolygonVertex;
				}
				else {
					normalIndex = indexArray.GetAt(indexByPolygonVertex);	// eIndexToDirectの場合
				}
				FbxVector4 normal = directArray.GetAt(normalIndex);		// 法線情報を取得
				if (normal[3] != 0.0f) {
					if (normal[3] == 1.0)
					{
						normalList.push_back(Vector3(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])));		// 同次座標へ
					}
					else {
						normalList.push_back(Vector3(static_cast<float>(normal[0] / normal[3]), static_cast<float>(normal[1] / normal[3]), static_cast<float>(normal[2] / normal[3])));	// 同次座標へ
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

	int elementCount = Mesh_->GetElementUVCount();		// 何個のＵＶ情報がセットされているか
	if (UvNo_ + 1 > elementCount)						// 今回は１個にのみ対応
	{
		return uvList;
	}

	FbxGeometryElementUV* element = Mesh_->GetElementUV(UvNo_);					// UVセットを取得
	FbxLayerElement::EMappingMode mappingMode = element->GetMappingMode();		// マッピングモードを取得
	FbxLayerElement::EReferenceMode referenceMode = element->GetReferenceMode();	// リファレンスモードを取得	
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// ＵＶ情報を格納したＶＥＣＴＯＲ配列のインデックス配列を取得
	const FbxLayerElementArrayTemplate<FbxVector2>& directArray = element->GetDirectArray();	// ＵＶ値配列を取得

	// eDirctかeIndexDirectのみ対応
	assert((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

	// 頂点インデックス数分ｖｅｃｔｏｒ容量を確保
	uvList.reserve(IndexList_.size());

	// 頂点に対応して格納されている場合
	if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		// 頂点座標でマッピング
		for (unsigned int i = 0; i < IndexList_.size(); i++)
		{
			int index = IndexList_[i];			// 面の構成情報配列から頂点インデックス番号を取得

			// リファレンスモードを判定
			int uvIndex;
			if (referenceMode == FbxGeometryElement::eDirect) {		// eDirectの場合
				uvIndex = index;		//　eDirectの場合（頂点インデックスと同じインデックス値でセットされている）
			}
			else {													// eIndexToDirectの場合
				uvIndex = indexArray.GetAt(index);				// 頂点座標インデックスに対応したＵＶ情報インデックスを取得
			}

			FbxVector2 uv = directArray.GetAt(uvIndex);		// uv値をdouble型で取得
			uvList.push_back(Vector2(static_cast<float>(uv[0]), static_cast<float>(uv[1])));		// float値として格納
		}
	}
	// 面の構成情報に対応して格納されている場合
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
	{
		// ポリゴンバーテックス（面の構成情報のインデックス）でマッピング
		int indexByPolygonVertex = 0;						// 面の構成情報インデックス配列のインデックス
		int polygonCount = Mesh_->GetPolygonCount();			// メッシュのポリゴン数を取得
		for (int i = 0; i < polygonCount; ++i)				// ポリゴン数分ループ
		{
			int polygonSize = Mesh_->GetPolygonSize(i);		// ｉ番目のポリゴン頂点数を取得

			// ポリゴンの頂点数分ループ
			for (int j = 0; j < polygonSize; ++j)
			{
				// リファレンスモードの判定？
				int uvIndex;
				if (referenceMode == FbxGeometryElement::eDirect) {		// eDirectの場合
					uvIndex = indexByPolygonVertex;		//　eDirectの場合（頂点インデックスと同じインデックス値でセットされている）
				}
				else {													// eIndexToDirectの場合
					uvIndex = indexArray.GetAt(indexByPolygonVertex);	// 面の構成情報インデックスに対応したＵＶ情報インデックスを取得
				}
				FbxVector2 uv = directArray.GetAt(uvIndex);

				uvList.push_back(Vector2(static_cast<float>(uv[0]), static_cast<float>(uv[1])));	// ｆｌｏａｔ値として格納

				++indexByPolygonVertex;						// 頂点インデックスをインクリメント
			}
		}
	}
	else
	{
		// それ以外のマッピングモードには対応しない
		assert(false);
	}

	return uvList;
}

void CFbxModel::GetWeight(FbxMesh* Mesh_, const std::vector<int>& IndexList_, std::vector<ModelBoneWeight>& BoneWeightList_, std::vector<std::string>& BoneNodeNameList_, std::vector<Matrix4x4>& InvBaseposeMatrixList_) {
	// メッシュ内のスキン情報数を取得する
	int skinCount = Mesh_->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount == 0)
	{
		return;
	}

	// スキンが1つにしか対応しない
	assert(skinCount <= 1);

	int controlPointsCount = Mesh_->GetControlPointsCount();		// メッシュの頂点数を取得
//	using TmpWeight = std::pair<int, float>;					// std::pair *=> 2つのものの入れ物(vs2010でエラー)
																// using TmpWeightでint とfloatの入れ物をＴｍｐＷｅｉｇｈｔと定義したことになる
	std::vector<std::vector<std::pair<int, float>>> tmpBoneWeightList(controlPointsCount);	// ボーンインデックス値とウェイト値をペアにしたＶＥＣＴＯＲ

	// ＦｂｘＭｅｓｈからスキン情報を取り出す
	FbxSkin* skin = static_cast<FbxSkin*>(Mesh_->GetDeformer(0, FbxDeformer::eSkin));
	int clusterCount = skin->GetClusterCount();	// スキンからクラスタ（ボーン）数を取得

	// クラスター（ボーンの事）の数分ループ
	for (int i = 0; i < clusterCount; ++i)
	{
		FbxCluster* cluster = skin->GetCluster(i);					// i番目のクラスタ（ボーン）を取得

		// eNormalizeしか対応しない
		assert(cluster->GetLinkMode() == FbxCluster::eNormalize);	// 正規化されていないウェイト値は対応していない

		BoneNodeNameList_.push_back(cluster->GetLink()->GetName());	// ボーン名を格納

		int indexCount = cluster->GetControlPointIndicesCount();	// ボーンに所属する頂点インデックス数を取得
		int* indices = cluster->GetControlPointIndices();			// 頂点インデックス配列を取得
		double* weights = cluster->GetControlPointWeights();		// ウェイト値配列を取得

		// ボーンに所属する頂点インデックス数分ループする
		for (int j = 0; j < indexCount; ++j)
		{
			int controlPointIndex = indices[j];		// このボーンに影響を受けるj番目の頂点インデックス番号を取得
			tmpBoneWeightList[controlPointIndex].push_back(std::pair<int, float>(i, static_cast<float>(weights[j])));
			// 頂点インデックス番号のＶＥＣＴＯＲ位置に（i番目のボーンとそのウエイト値）
		}

		// ペースポーズの逆行列を作成しておく
		Matrix4x4 invBaseposeMatrix;

		FbxMatrix baseposeMatrix = cluster->GetLink()->EvaluateGlobalTransform().Inverse();		// 逆行列を計算する

		// FbxMatrix->Matrix4x4へ
		auto baseposeMatrixPtr = (double*)baseposeMatrix;
		for (int j = 0; j < 16; ++j)
		{
			invBaseposeMatrix.m[j] = static_cast<float>(baseposeMatrixPtr[j]);	// double->float
		}

		InvBaseposeMatrixList_.push_back(invBaseposeMatrix);			// i番目のボーンの初期姿勢を示す逆行列
	}

	// コントロールポイントに対応したウェイトを作成
	std::vector<ModelBoneWeight> boneWeightListControlPoints;

	for (unsigned int j = 0; j < tmpBoneWeightList.size(); j++) {
		// ｊ番目の頂点のウェイト値を取得
		std::vector<std::pair<int, float>>& tmpBoneWeight = tmpBoneWeightList[j];

		// ウェイト値の大きさでソート
		std::sort(tmpBoneWeight.begin(), tmpBoneWeight.end(),
			[](const std::pair<int, float>& weightA, const std::pair<int, float>& weightB) { return weightA.second > weightB.second; }
		);

		// 1頂点に4つより多くウェイトが割り振られているなら影響が少ないものは無視する
		while (tmpBoneWeight.size() > 4)
		{
			tmpBoneWeight.pop_back();
		}

		// 4つに満たない場合はダミーを挿入
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

		// ウェイトの正規化
		for (int i = 0; i < 4; ++i)
		{
			weight.boneWeight.v[i] /= total;
		}

		boneWeightListControlPoints.push_back(weight);


	}

	// インデックスで展開
	for (unsigned int j = 0; j < IndexList_.size(); j++) {
		int index = IndexList_[j];				// j番目の頂点インデックスを取得
		BoneWeightList_.push_back(boneWeightListControlPoints[index]);	// j番目の頂点のボーン番号とウエイト値をセット
	}
}

bool CFbxModel::CreateVertexBuffer(unsigned int Vertexnum_, void* Vertexdata_, ID3D11Buffer** pVertexBuffer_) {
	// 頂点バッファ生成
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

	// インデックスバッファ生成
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
	// コンスタントバッファ生成
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

	//移動----

	//--------
	XMFLOAT3 angle = { 0,0,0 };
	XMFLOAT3 trans = { 0,0,100 };
	MakeWorldMatrix(g_World, angle, trans);

	//
	// コンスタントバッファ内の行列を更新
	//
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);					// 転置して設定（ワールド変換行列）
	cb.mView = XMMatrixTranspose(g_View);						// 転置して設定（カメラ変換行列）
	cb.mProjection = XMMatrixTranspose(g_Projection);			// 転置して設定（プロジェクション変換行列）
	cb.mLightDirection = XMFLOAT3(1, 1, 1);						// 光の方向ベクトルセット	
	cb.mCameraPos.x = 0.0f;										// カメラの位置
	cb.mCameraPos.y = 0.0f;
	cb.mCameraPos.z = -1500.0f;
	GetDX11DeviceContext()->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);	// 更新

	// Zバッファクリア
	GetDX11DeviceContext()->ClearDepthStencilView(GetDX11Obj()->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// シェーダーセット
	GetDX11DeviceContext()->VSSetShader(m_pVertexShader, nullptr, 0);			// 頂点シェーダーをセット
	GetDX11DeviceContext()->PSSetShader(m_pPixelShader, nullptr, 0);			// ピクセルシェーダーをセット

	GetDX11DeviceContext()->IASetInputLayout(m_pVertexLayout);

	// コンスタントバッファをセット（）
	GetDX11DeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);	// コンスタントバッファをセット

	// サンプラーのセット
	GetDX11DeviceContext()->PSSetSamplers(0, 1, &m_pSampleLinear);				// サンプラーのセット

	// プリミティブをセット
	GetDX11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// UNITYちゃん(複数のメッシュの集合体として出来上がっている)
	for (unsigned int i = 0; i < m_MeshList.size(); i++) {

		unsigned int stride = sizeof(SimpleVertex);
		unsigned  offset = 0;

		// ワンスキンのメッシュでない場合
		if (m_MeshList[i].skincount == 0)
		{
			getMeshmatrix(m_Frame,					// フレーム数	
				i,									// メッシュＩＤ
				m_BoneMatrixList[0]);

			ConstantBufferBoneMatrix cb;
			for (int k = 0; k < 1; k++)
			{
				cb.mBoneMatrix[k] = XMMATRIX(
					m_BoneMatrixList[k].m[0], m_BoneMatrixList[k].m[1], m_BoneMatrixList[k].m[2], m_BoneMatrixList[k].m[3],
					m_BoneMatrixList[k].m[4], m_BoneMatrixList[k].m[5], m_BoneMatrixList[k].m[6], m_BoneMatrixList[k].m[7],
					m_BoneMatrixList[k].m[8], m_BoneMatrixList[k].m[9], m_BoneMatrixList[k].m[10], m_BoneMatrixList[k].m[11],
					m_BoneMatrixList[k].m[12], m_BoneMatrixList[k].m[13], m_BoneMatrixList[k].m[14], m_BoneMatrixList[k].m[15]);

				cb.mBoneMatrix[k] = XMMatrixTranspose(cb.mBoneMatrix[k]);		// 転置しないとおかしい
			}
			GetDX11DeviceContext()->UpdateSubresource(m_pConstantBufferBoneMatrix, 0, nullptr, &cb, 0, 0);	// 更新

			// コンスタントバッファをセット(ボーン行列)
			GetDX11DeviceContext()->VSSetConstantBuffers(1, 1, &m_pConstantBufferBoneMatrix);	// コンスタントバッファをセット（ボーン行列）


			// マテリアル名を取得
			std::string materialname = m_MeshList[i].materialName;

			// このマテリアル名を持つマテリアル情報を検索
			ModelMaterial material;
			material = GetMaterial(materialname);

			//
			// コンスタントバッファ内のマテリアルを更新
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

			GetDX11DeviceContext()->UpdateSubresource(m_pConstantBufferMaterial, 0, nullptr, &cbm, 0, 0);	// 更新

			// コンスタントバッファをセット(ボーン行列)
			GetDX11DeviceContext()->VSSetConstantBuffers(2, 1, &m_pConstantBufferMaterial);	// コンスタントバッファをセット（マテリアル）

			// 頂点バッファセット
			GetDX11DeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer[i], &stride, &offset);

			// インデックスバッファをセット
			GetDX11DeviceContext()->IASetIndexBuffer(m_pIndexBuffer[i], DXGI_FORMAT_R16_UINT, 0);

			// テクスチャをセット
			GetDX11DeviceContext()->PSSetShaderResources(0, 1, &m_Texture[i]);			// テクスチャをセット

			// 描画
			GetDX11DeviceContext()->DrawIndexed(m_MeshList[i].indexList.size(), 0, 0);
		}
		// ワンスキンのメッシュの場合
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

				cb.mBoneMatrix[k] = XMMatrixTranspose(cb.mBoneMatrix[k]);		// 転置しないとおかしい
			}
			GetDX11DeviceContext()->UpdateSubresource(m_pConstantBufferBoneMatrix, 0, nullptr, &cb, 0, 0);	// 更新

			// コンスタントバッファをセット(ボーン行列)
			GetDX11DeviceContext()->VSSetConstantBuffers(1, 1, &m_pConstantBufferBoneMatrix);	// コンスタントバッファをセット（ボーン行列）

			// マテリアル名を取得
			std::string materialname = m_MeshList[i].materialName;
			ConstantBufferMaterial cbm;
			if (materialname != "noName")
			{

				// このマテリアル名を持つマテリアル情報を検索
				static ModelMaterial material;
				material = GetMaterial(materialname);

				//
				// コンスタントバッファ内のマテリアルを更新
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

			GetDX11DeviceContext()->UpdateSubresource(m_pConstantBufferMaterial, 0, nullptr, &cbm, 0, 0);	// 更新

			// コンスタントバッファをセット（マテリアル）
			GetDX11DeviceContext()->VSSetConstantBuffers(2, 1, &m_pConstantBufferMaterial);

			// 頂点バッファセット
			GetDX11DeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer[i], &stride, &offset);

			// インデックスバッファをセット
			GetDX11DeviceContext()->IASetIndexBuffer(m_pIndexBuffer[i], DXGI_FORMAT_R16_UINT, 0);

			// テクスチャをセット
			GetDX11DeviceContext()->PSSetShaderResources(0, 1, &m_Texture[i]);			// テクスチャをセット

			// 描画
			GetDX11DeviceContext()->DrawIndexed(m_MeshList[i].indexList.size(), 0, 0);
		}
	}

	m_Frame++;

	// アニメーション終了したら最初から
	if (isAnimationEnd(m_Frame))
	{
		m_Frame = 0;
	}
}