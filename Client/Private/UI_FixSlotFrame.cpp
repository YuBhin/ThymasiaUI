#include "pch.h" 
#include "UI_FixSlotFrame.h"
#include "GameInstance.h"

CUI_FixSlotFrame::CUI_FixSlotFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Image{ pDevice, pContext }
{
}

CUI_FixSlotFrame::CUI_FixSlotFrame(const CUI_FixSlotFrame& Prototype)
	: CUI_Image(Prototype)
{
}

HRESULT CUI_FixSlotFrame::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_FixSlotFrame::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_FixSlotFrame::Priority_Update(_float fTimeDelta)
{
}

void CUI_FixSlotFrame::Update(_float fTimeDelta)
{
}

void CUI_FixSlotFrame::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_FixSlotFrame::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bIconOn", &m_bSkillIconOn, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bTexEffectOff", &m_bEffectOn, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_SlotNum", &m_iTexNumber, sizeof(_uint))))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;
	if (FAILED(m_pTexIconCom->Bind_ShaderResource(m_pShaderCom, "g_TexIcon", m_iTexicon)))
		return E_FAIL;
	if (FAILED(m_pTexEffectCom->Bind_ShaderResource(m_pShaderCom, "g_TexEffect", m_iTexEffect)))
		return E_FAIL;


	m_pShaderCom->Begin(m_iShaderPassNum);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_FixSlotFrame::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_PlunderSlotFrame"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/*Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Effect_SlotFrame"),
		TEXT("Com_TexEffect"), reinterpret_cast<CComponent**>(&m_pTexEffectCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SkillIcon"),
		TEXT("Com_TexIcon"), reinterpret_cast<CComponent**>(&m_pTexIconCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	return S_OK;
}

CUI_FixSlotFrame* CUI_FixSlotFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_FixSlotFrame* pInstance = new CUI_FixSlotFrame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_FixSlotFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_FixSlotFrame::Clone(void* pArg)
{
	CUI_FixSlotFrame* pInstance = new CUI_FixSlotFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_FixSlotFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_FixSlotFrame::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTexIconCom);
	Safe_Release(m_pTexEffectCom);
}
