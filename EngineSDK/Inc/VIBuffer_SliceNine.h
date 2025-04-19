#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_SliceNine final : public CVIBuffer
{
private:
	CVIBuffer_SliceNine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_SliceNine(const CVIBuffer_SliceNine& Prototype);
	virtual ~CVIBuffer_SliceNine() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:

public:
	static CVIBuffer_SliceNine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:

};

END