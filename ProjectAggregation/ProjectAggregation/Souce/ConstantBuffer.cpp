#include "ConstantBuffer.h"
#include "CDirectxGraphics.h"

#define STS_ifERROR_FUNCTION 	if (!sts) { MessageBox(NULL, "AnyFunction is Error", "Error", MB_OK); return;}

void ConstantBufferManager::Init() {

	//時間を流す定数バッファ作成
	bool sts = CreateConstantBuffer(
		CDirectXGraphics::GetInstance()->GetDXDevice(),
		sizeof(ConstantBufferStates::ConstantBufferTime),
		&m_ConstantBuffer
	);
	STS_ifERROR_FUNCTION
	m_ConstantBufferStates.m_ConstantBufferTime.Padding[0] = 0;
	m_ConstantBufferStates.m_ConstantBufferTime.Padding[1] = 0;
	m_ConstantBufferStates.m_ConstantBufferTime.Padding[2] = 0;
}

void ConstantBufferManager::UnInit() {
	if (m_ConstantBuffer != nullptr) {
		m_ConstantBuffer->Release();
		m_ConstantBuffer = nullptr;
	}
}

template <typename TConstantStruct>
void ConstantBufferManager::UpdateConstantBuffer(TConstantStruct* ConstantStructBuffer, int RegistarNum) {

	switch (RegistarNum)
	{
	case 6:
		m_ConstantBufferStates.m_ConstantBufferTime.iTime = static_cast<float>(timeGetTime());
		CDirectXGraphics::GetInstance()->GetImmediateContext()->UpdateSubresource(
			m_ConstantBuffer,
			0,
			NULL,
			&m_ConstantBufferStates.m_ConstantBufferTime,
			0,
			0
		);

		CDirectXGraphics::GetInstance()->GetImmediateContext()->PSSetConstantBuffers(
			6,
			1,
			&m_ConstantBuffer
		);
		break;

	default:
		break;
	}
}

void ConstantBufferManager::UpdateConstantBuffer(int RegisterNum) {
	switch (RegisterNum)
	{
	case 6:
		m_ConstantBufferStates.m_ConstantBufferTime.iTime = static_cast<float>(timeGetTime());
		CDirectXGraphics::GetInstance()->GetImmediateContext()->UpdateSubresource(
			m_ConstantBuffer,
			0,
			NULL,
			&m_ConstantBufferStates.m_ConstantBufferTime,
			0,
			0
		);

		CDirectXGraphics::GetInstance()->GetImmediateContext()->PSSetConstantBuffers(
			6,
			1,
			&m_ConstantBuffer
		);
		break;

	default:
		break;
	}
}