#pragma once
#include <string>
#include <vector>
#include "definer.h"
//PMDモデルを読み込むクラス
//モデル情報の仕様に関してはリソースファイルの[PMX仕様]を確認

//モデルデータ型定義
struct PMXModelData {

	static constexpr int NO_DATA = -1;
	//頂点
	typedef struct{
		XMFLOAT3 Pos;//座標
		XMFLOAT3 Nor;//法線
		XMFLOAT2 UV;//UV座標
		std::vector<XMFLOAT4> AddUV;//追加UV

		//ボーンウェイト
		typedef struct {
			enum TypeE {
				BDEF1,
				BDEF2,
				BDEF4,
				SDEF,
			};

			TypeE Type;
			//ボーン
			int Born1;
			int Born2;
			int Born3;
			int Born4;
			//ウェイト
			float Weight1;
			float Weight2;
			float Weight3;
			float Weight4;
			XMFLOAT3 C;
			XMFLOAT3 R0;
			XMFLOAT3 R1;
		}WeightT;
		WeightT Weight;

		//エッジ倍率
		float EdgeMagF;
	}VertexT;

	//面
	typedef struct{
		int VertexIndex;
	} SurfaceT;

	//マテリアル
	typedef struct
	{
		XMFLOAT4 Diffuse;//ディフューズ
		XMFLOAT3 Specular;//スペキュラ
		float Specularity;
		XMFLOAT3 Ambient;//アンビエント

		int ColorMapTextureIndex;//カラーマップテクスチャ
		int ToonTextureIndex;//トゥーンテクスチャ

		int VertexNum;//頂点数
	} MaterialT;

	//ボーン
	struct BoneT {
		std::wstring Name;//名前
		std::string NameByEnglish;//英名

		XMFLOAT3 Pos;//座標
		int ParentIndex;//親情報
		int TransLevel;//変形レベル
		unsigned short Flag;
		XMFLOAT3 CoordOffset;//座標オフセット
		int ChildrenIndex;//子情報
		int ImpartParentIndex;
		float ImpartRate;

		XMFLOAT3 FixedAxis;//固定軸方向ベクトル
		XMFLOAT3 LocalAxisX;//ローカルX軸方向ベクトル
		XMFLOAT3 LocalAxisZ;//ローカルZ軸方向ベクトル

		int ExternalParentKey;//外部親情報
		int IkTargetIndex;//IKターゲット情報
		int IkLoopCount;
		float IkUnitAngle;

		//IK情報
		 struct IkLinkT {
			int Index;
			bool ExistAngleLimited;
			XMFLOAT3 LimitAngleMin[3];
			XMFLOAT3 LimitAngleMax[3];
		};

		std::vector<IkLinkT> IkLink;

	};

	std::vector<VertexT> vertices;
	std::vector<SurfaceT> surfaces;
	std::vector<std::wstring> texturePaths;
	std::vector<MaterialT> materials;
	std::vector<BoneT> bones;
};

class CPmdLoader {
private:
	PMXModelData m_ModelData{};
public:

	CPmdLoader(std::wstring& FileName);//コンストラクタ
	~CPmdLoader();//デストラクタ
};