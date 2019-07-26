#include "PmdLoader.h"
#include <fstream>
#include <array>
#include <Windows.h>

bool getPMXStringUTF16(std::ifstream& _file, std::wstring& output)
{
	std::array<wchar_t, 512> wBuffer{};
	int textSize;

	_file.read(reinterpret_cast<char*>(&textSize), 4);

	_file.read(reinterpret_cast<char*>(&wBuffer), textSize);
	output = std::wstring(&wBuffer[0], &wBuffer[0] + textSize / 2);

	return true;
}


CPmdLoader::CPmdLoader(std::wstring& FileName) {
	//コンストラクタ
	if (FileName.empty() == true) {
		return;
	}

	//モデルフォルダのパスを取得
	std::wstring FolderPath(FileName.begin(), FileName.begin() + FileName.rfind(L'\\') + 1);

	//ファイル読み込み
	std::ifstream PmxFile(FolderPath, (std::ios::binary | std::ios::in));
	if (PmxFile.fail() == true) {
		PmxFile.close();
		return;
	}

	//ヘッダー
	std::array<byte, 4> PmxHeader{};
	constexpr std::array<byte, 4> PMX_MAGIC_NUMBER{ 0x50,0x4d,0x58,0x20 };
	enum HeaderDataE {
		ENCORDING_FORMAT,
		NUMBER_ADD_UV,
		VERTEX_INDEX_SIZE,
		TEXTURE_INDEX_SIZE,
		MATERIAL_INDEX_SIZE,
		BONE_INDEX_SIZE,
		RIGID_BODY_INDEX_SIZE
	};

	for (int i = 0; i < 4; i++) {
		PmxHeader[i] = PmxFile.get();
	}
	if (PmxHeader != PMX_MAGIC_NUMBER) {
		PmxFile.close();
		return;
	}

	//バージョン2.0のみ
	float Version{};
	PmxFile.read(reinterpret_cast<char*>(&Version), 4);
	if (!XMScalarNearEqual(Version, 2.0f, g_XMEpsilon.f[0])) {
		PmxFile.close();
		return;
	}

	byte HeaderDataLength = PmxFile.get();
	if (HeaderDataLength != 8) {
		PmxFile.close();
		return;
	}
	std::array<byte, 8> HeaderData{};
	for (int i = 0; i < HeaderDataLength; i++) {
		HeaderData[i] = PmxFile.get();
	}
	if (HeaderData[0] != 0) {
		PmxFile.close();
		return;
	}

	unsigned ArrayLength{};
	for (int i = 0; i < 4; i++) {
		PmxFile.read(reinterpret_cast<char*>(&ArrayLength), 4);
		for (unsigned j = 0; j < ArrayLength; j++) {
			PmxFile.get();
		}
	}
	//頂点--------------------------------------------------------------------------------------------
	using Vertex = PMXModelData::VertexT;
	int NumberOfVertex{};
	PmxFile.read(reinterpret_cast<char*>(&NumberOfVertex), 4);
	m_ModelData.vertices.resize(NumberOfVertex);

	//それぞれのバイト数指定して読み込んでいく
	for (int i = 0; i < NumberOfVertex; i++) {
		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Pos), 12);
		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Nor), 12);
		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].UV), 8);

		if (HeaderData[NUMBER_ADD_UV] != 0) {

			for (int j = 0; j < HeaderData[NUMBER_ADD_UV]; j++)
			{
				PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].AddUV[j]), 16);
			}
		}

		const byte WeightMethod = PmxFile.get();

		//ボーン設定読み込み
		switch (WeightMethod) {
		case Vertex::WeightT::BDEF1:
			m_ModelData.vertices[i].Weight.Type = Vertex::WeightT::BDEF1;
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Born1), HeaderData[BONE_INDEX_SIZE]);
			m_ModelData.vertices[i].Weight.Born2 = NO_DATA;
			m_ModelData.vertices[i].Weight.Born3 = NO_DATA;
			m_ModelData.vertices[i].Weight.Born4 = NO_DATA;

			m_ModelData.vertices[i].Weight.Weight1 = 1.0f;
			break;

		case Vertex::WeightT::BDEF2:
			m_ModelData.vertices[i].Weight.Type = Vertex::WeightT::BDEF2;
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Born1), HeaderData[BONE_INDEX_SIZE]);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Born2), HeaderData[BONE_INDEX_SIZE]);
			m_ModelData.vertices[i].Weight.Born3 = NO_DATA;
			m_ModelData.vertices[i].Weight.Born4 = NO_DATA;

			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Weight1), 4);
			m_ModelData.vertices[i].Weight.Weight2 = 1.0f - m_ModelData.vertices[1].Weight.Weight1;
			break;

		case Vertex::WeightT::BDEF4:
			m_ModelData.vertices[i].Weight.Type = Vertex::WeightT::BDEF4;
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Born1), HeaderData[BONE_INDEX_SIZE]);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Born2), HeaderData[BONE_INDEX_SIZE]);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Born3), HeaderData[BONE_INDEX_SIZE]);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Born4), HeaderData[BONE_INDEX_SIZE]);

			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Weight1), 4);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Weight2), 4);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Weight3), 4);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Weight4), 4);
			break;

		case Vertex::WeightT::SDEF:
			m_ModelData.vertices[i].Weight.Type = Vertex::WeightT::SDEF;
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Born1), HeaderData[BONE_INDEX_SIZE]);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Born2), HeaderData[BONE_INDEX_SIZE]);
			m_ModelData.vertices[i].Weight.Born3 = NO_DATA;
			m_ModelData.vertices[i].Weight.Born4 = NO_DATA;

			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.Weight1), 4);
			m_ModelData.vertices[i].Weight.Weight2 = 1.0f - m_ModelData.vertices[i].Weight.Weight1;

			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.C), 12);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.R0), 12);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].Weight.R1), 12);
			break;

		default:
			PmxFile.close();
			return;
			break;
		}

		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.vertices[i].EdgeMagF), 4);

		if (m_ModelData.vertices[i].Weight.Born1 == NO_DATA) {
			PmxFile.close();
			return;
		}
	}

	//-----------------------------------------------------------------------------------------------

	//面----------------------------------------------------------------------------------------------
	int NumOfSurface{};
	PmxFile.read(reinterpret_cast<char*>(&NumOfSurface), 4);
	m_ModelData.surfaces.resize(NumOfSurface);

	for (int i = 0; i < NumOfSurface; i++) {
		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.surfaces[i].VertexIndex), HeaderData[VERTEX_INDEX_SIZE]);

		if (m_ModelData.surfaces[i].VertexIndex == NO_DATA || m_ModelData.surfaces[i].VertexIndex == NO_DATA || m_ModelData.surfaces[i].VertexIndex == NO_DATA)
		{
			PmxFile.close();
			return;
		}

	}

	//-----------------------------------------------------------------------------------------------

	//テクスチャ---------------------------------------------------------------------------------------
	int NumOfTexture{};
	PmxFile.read(reinterpret_cast<char*>(&NumOfTexture), 4);
	m_ModelData.texturePaths.resize(NumOfTexture);

	std::wstring texturePath{};
	for (int i = 0; i < NumOfTexture; i++)
	{
		m_ModelData.texturePaths[i] = FolderPath;
		getPMXStringUTF16(PmxFile, texturePath);
		m_ModelData.texturePaths[i] += texturePath;
	}

	//-----------------------------------------------------------------------------------------------

	//マテリアル---------------------------------------------------------------------------------------
	int NumOfMaterial{};
	PmxFile.read(reinterpret_cast<char*>(&NumOfMaterial), 4);

	m_ModelData.materials.resize(NumOfMaterial);
	for (int i = 0; i < NumOfMaterial; i++)
	{
		for (int j = 0; j < 2; ++j)
		{
			PmxFile.read(reinterpret_cast<char*>(&ArrayLength), 4);
			for (unsigned i = 0; i < ArrayLength; i++)
			{
				PmxFile.get();
			}
		}

		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.materials[i].Diffuse), 16);
		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.materials[i].Specular), 12);
		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.materials[i].Specularity), 4);
		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.materials[i].Ambient), 12);

		PmxFile.get();
		for (int i = 0; i < 16; i++)
		{
			PmxFile.get();
		}
		for (int i = 0; i < 4; i++)
		{
			PmxFile.get();
		}

		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.materials[i].ColorMapTextureIndex), HeaderData[TEXTURE_INDEX_SIZE]);
		for (unsigned char i = 0; i < HeaderData[TEXTURE_INDEX_SIZE]; i++)
		{
			PmxFile.get();
		}
		PmxFile.get();

		const byte shareToonFlag = PmxFile.get();
		if (shareToonFlag)
		{
			PmxFile.get();
		}
		else
		{
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.materials[i].ToonTextureIndex), HeaderData[TEXTURE_INDEX_SIZE]);
		}

		PmxFile.read(reinterpret_cast<char*>(&ArrayLength), 4);
		for (unsigned i = 0; i < ArrayLength; i++)
		{
			PmxFile.get();
		}

		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.materials[i].VertexNum), 4);
	}
		
	//-----------------------------------------------------------------------------------------------

	//ボーン------------------------------------------------------------------------------------------
	int numOfBone{};
	PmxFile.read(reinterpret_cast<char*>(&numOfBone), 4);

	m_ModelData.bones.resize(numOfBone);
	int ikLinkSize = 0;
	unsigned char angleLim = 0;

	for (int i = 0; i < numOfBone; i++)
	{
		getPMXStringUTF16(PmxFile, m_ModelData.bones[i].Name);
		PmxFile.read(reinterpret_cast<char*>(&ArrayLength), 4);
		m_ModelData.bones[i].NameByEnglish.resize(ArrayLength);
		for (unsigned j = 0; j < ArrayLength; ++j)
		{
			m_ModelData.bones[i].NameByEnglish[j] = PmxFile.get();
		}

		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].Pos), 12);

		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].ParentIndex), HeaderData[BONE_INDEX_SIZE]);
		if (numOfBone <= m_ModelData.bones[i].ParentIndex)
		{
			m_ModelData.bones[i].ParentIndex = NO_DATA;
		}

		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].TransLevel), 4);

		PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].Flag), 2);

		enum BoneFlagMask
		{
			ACCESS_POINT = 0x0001,
			IK = 0x0020,
			IMPART_TRANSLATION = 0x0100,
			IMPART_ROTATION = 0x0200,
			AXIS_FIXING = 0x0400,
			LOCAL_AXIS = 0x0800,
			EXTERNAL_PARENT_TRANS = 0x2000,
		};

		if (m_ModelData.bones[i].Flag & ACCESS_POINT)
		{
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].ChildrenIndex), HeaderData[BONE_INDEX_SIZE]);
			if (numOfBone <= m_ModelData.bones[i].ChildrenIndex)
			{
				m_ModelData.bones[i].ChildrenIndex = NO_DATA;
			}
		}
		else
		{
			m_ModelData.bones[i].ChildrenIndex = NO_DATA;
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].CoordOffset), 12);
		}
		if ((m_ModelData.bones[i].Flag & IMPART_TRANSLATION) || (m_ModelData.bones[i].Flag & IMPART_ROTATION))
		{
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].ImpartParentIndex), HeaderData[BONE_INDEX_SIZE]);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].ImpartRate), 4);
		}
		if (m_ModelData.bones[i].Flag & AXIS_FIXING)
		{
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].FixedAxis), 12);
		}
		if (m_ModelData.bones[i].Flag & LOCAL_AXIS)
		{
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].LocalAxisX), 12);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].LocalAxisZ), 12);
		}
		if (m_ModelData.bones[i].Flag & EXTERNAL_PARENT_TRANS)
		{
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].ExternalParentKey), 4);
		}
		if (m_ModelData.bones[i].Flag & IK)
		{
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].IkTargetIndex), HeaderData[5]);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].IkLoopCount), 4);
			PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].IkUnitAngle), 4);
			PmxFile.read(reinterpret_cast<char*>(&ikLinkSize), 4);
			m_ModelData.bones[i].IkLink.resize(ikLinkSize);
			for (int j = 0; j < ikLinkSize; ++j)
			{
				PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].IkLink[j].Index), HeaderData[5]);
				angleLim = PmxFile.get();
				m_ModelData.bones[i].IkLink[j].ExistAngleLimited = false;
				if (angleLim == 1)
				{
					PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].IkLink[j].LimitAngleMin), 12);
					PmxFile.read(reinterpret_cast<char*>(&m_ModelData.bones[i].IkLink[j].LimitAngleMax), 12);
					m_ModelData.bones[i].IkLink[j].ExistAngleLimited = true;
				}
			}
		}
		else
		{
			m_ModelData.bones[i].IkTargetIndex = NO_DATA;
		}
	}

	//-----------------------------------------------------------------------------------------------
	PmxFile.close();
	return;

}