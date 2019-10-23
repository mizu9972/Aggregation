#pragma once
#include "Shader.h"



class ConstantBufferManager {
private:
	ConstantBufferManager() {};
	~ConstantBufferManager() = default;

	ID3D11Buffer* m_ConstantBuffer;

	class ConstantBufferStates {
		// 追加の定数バッファ定義
	public:
		//6
		ALIGN16 struct ConstantBufferTime {
			float iTime;
			float Padding[3];
		};

		ConstantBufferTime m_ConstantBufferTime;
	};
	ConstantBufferStates m_ConstantBufferStates;
public:
	ConstantBufferManager(const ConstantBufferManager&) = delete;
	ConstantBufferManager(ConstantBufferManager&&) = delete;
	ConstantBufferManager operator = (const ConstantBufferManager&) = delete;
	ConstantBufferManager operator = (ConstantBufferManager&&) = delete;

	static ConstantBufferManager* GetInstance() {
		static ConstantBufferManager Instance;
		return &Instance;
	}

	//レジスタリスト
	enum class RegistarList {
		Time = 6,
	};

	void Init();
	void UnInit();

	//コンスタントバッファ更新
	template <typename TConstantStruct>
	void UpdateConstantBuffer(TConstantStruct* ConstantStructBuffer, int RegistarNum);
	void UpdateConstantBuffer(int RegistarNum);

	template <typename TConstantStruct>	
	void UpdateConstantBuffer(TConstantStruct* ConstantStructBuffer, RegistarList RegistarNum) {
		UpdateConstantBuffer(ConstantStructBuffer, static_cast<int>(RegistarNum));
	}
	void UpdateConstantBuffer(RegistarList RegistarNum) {
		UpdateConstantBuffer(static_cast<int>(RegistarNum));
	}
};