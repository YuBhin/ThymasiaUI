#pragma once
#include "Client_Defines.h"
#include "UI_Button.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Frame final : public CUI_Button
{
public:
	enum TexKind{TEX_SLOT,TEX_ICON,TEX_EDGE,TEX_EFFECT,TEX_END};
private:
	CUI_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Frame(const CUI_Frame& Prototype);
	virtual ~CUI_Frame() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:

	CTexture* m_pTextureCom[TEX_END] = {nullptr};

	_uint m_iTexIcon = { 1 };
	_uint m_iTexEdge = { 1 };
	_uint m_iTexEffect = {1 };

public:
	HRESULT Ready_Components();

public:
	static CUI_Frame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END