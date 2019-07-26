#pragma once
#include <string>
#include <vector>
#include "definer.h"
//PMD���f����ǂݍ��ރN���X
//���f�����̎d�l�Ɋւ��Ă̓��\�[�X�t�@�C����[PMX�d�l]���m�F

//���f���f�[�^�^��`
struct PMXModelData {

	static constexpr int NO_DATA = -1;
	//���_
	typedef struct{
		XMFLOAT3 Pos;//���W
		XMFLOAT3 Nor;//�@��
		XMFLOAT2 UV;//UV���W
		std::vector<XMFLOAT4> AddUV;//�ǉ�UV

		//�{�[���E�F�C�g
		typedef struct {
			enum TypeE {
				BDEF1,
				BDEF2,
				BDEF4,
				SDEF,
			};

			TypeE Type;
			//�{�[��
			int Born1;
			int Born2;
			int Born3;
			int Born4;
			//�E�F�C�g
			float Weight1;
			float Weight2;
			float Weight3;
			float Weight4;
			XMFLOAT3 C;
			XMFLOAT3 R0;
			XMFLOAT3 R1;
		}WeightT;
		WeightT Weight;

		//�G�b�W�{��
		float EdgeMagF;
	}VertexT;

	//��
	typedef struct{
		int VertexIndex;
	} SurfaceT;

	//�}�e���A��
	typedef struct
	{
		XMFLOAT4 Diffuse;//�f�B�t���[�Y
		XMFLOAT3 Specular;//�X�y�L����
		float Specularity;
		XMFLOAT3 Ambient;//�A���r�G���g

		int ColorMapTextureIndex;//�J���[�}�b�v�e�N�X�`��
		int ToonTextureIndex;//�g�D�[���e�N�X�`��

		int VertexNum;//���_��
	} MaterialT;

	//�{�[��
	struct BoneT {
		std::wstring Name;//���O
		std::string NameByEnglish;//�p��

		XMFLOAT3 Pos;//���W
		int ParentIndex;//�e���
		int TransLevel;//�ό`���x��
		unsigned short Flag;
		XMFLOAT3 CoordOffset;//���W�I�t�Z�b�g
		int ChildrenIndex;//�q���
		int ImpartParentIndex;
		float ImpartRate;

		XMFLOAT3 FixedAxis;//�Œ莲�����x�N�g��
		XMFLOAT3 LocalAxisX;//���[�J��X�������x�N�g��
		XMFLOAT3 LocalAxisZ;//���[�J��Z�������x�N�g��

		int ExternalParentKey;//�O���e���
		int IkTargetIndex;//IK�^�[�Q�b�g���
		int IkLoopCount;
		float IkUnitAngle;

		//IK���
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

	CPmdLoader(std::wstring& FileName);//�R���X�g���N�^
	~CPmdLoader();//�f�X�g���N�^
};