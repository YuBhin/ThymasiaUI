#pragma once
#include "Client_Defines.h"
#include "UI_Image.h"


BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Talent_Icon final : public CUI_Image
{
private:
	CUI_Talent_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Talent_Icon(const CUI_Talent_Icon& Prototype);
	virtual ~CUI_Talent_Icon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };



public:
	HRESULT Ready_Components();

public:
	static CUI_Talent_Icon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END