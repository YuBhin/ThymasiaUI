#pragma once

#include "Client_Defines.h"
#include "UI_Image.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_FixSlotFrame final : public CUI_Image
{
private:
	CUI_FixSlotFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_FixSlotFrame(const CUI_FixSlotFrame& Prototype);
	virtual ~CUI_FixSlotFrame() = default;

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

	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTexIconCom = { nullptr };
	CTexture* m_pTexEffectCom = { nullptr };


private:
	_uint m_iTexicon = { 1 };
	_uint m_iTexEffect = { 1 };

	_bool m_bEffectOn = { false };
	_bool m_bSkillIconOn = { true };


public:
	HRESULT Ready_Components();

public:
	static CUI_FixSlotFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END