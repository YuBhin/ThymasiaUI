#pragma once
#include "Client_Defines.h"
#include "UI_Image.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_DialogueTalkBackground final : public CUI_Image
{
private:
	CUI_DialogueTalkBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_DialogueTalkBackground(const CUI_DialogueTalkBackground& Prototype);
	virtual ~CUI_DialogueTalkBackground() = default;

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
	static CUI_DialogueTalkBackground* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END