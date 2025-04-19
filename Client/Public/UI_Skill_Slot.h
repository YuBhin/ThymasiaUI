#pragma once
#include "Client_Defines.h"
#include "UI_Button.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Skill_Slot final : public CUI_Button
{
public:
	enum TexKind { TEX_SLOT, TEX_ICON, TEX_EDGE, TEX_EFFECT, TEX_END };
private:
	CUI_Skill_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Skill_Slot(const CUI_Skill_Slot& Prototype);
	virtual ~CUI_Skill_Slot() = default;

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

	CTexture* m_pTextureCom[TEX_END] = { nullptr };

	_uint m_iTexSlot = { 0 }; // ������
	_uint m_iTexEdge = { 1 };
	_uint m_iTexEffect = { 0 };

private:
	_bool m_bTexIconOff = { false }; // Ư���� Ȱ��ȭ �Ǿ� ���� �ʴٸ� �̹����� ��Ӱ� ó���ϱ� ���� �뵵
	_bool m_bTexEdgeOff = { true }; // ȣ�� ��¦��¦
	_bool m_bTexEffectOff = { true }; // ����Ʈ �̹��� �׸��� ����

public:
	HRESULT Ready_Components();

public:
	static CUI_Skill_Slot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END