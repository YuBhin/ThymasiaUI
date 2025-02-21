#include "pch.h"
#include "Attribute_Button.h"
#include "GameInstance.h"

CAttribute_Button::CAttribute_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CAttribute_Button::CAttribute_Button(const CAttribute_Button& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CAttribute_Button::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAttribute_Button::Initialize(void* pArg)
{
	BUTTON_DESC* pDesc = static_cast<BUTTON_DESC*>(pArg);

	pDesc->fSizeX = 150.f;
	pDesc->fSizeY = 150.f;
	pDesc->fX = 600.f;
	pDesc->fY = 450.f;
	pDesc->fNear = 0.f;
	pDesc->fFar = 1.f;

	pDesc->fSpeedPerSec = 5.f;
	pDesc->fRotationPerSec = XMConvertToRadians(90.f);

	m_TexNumber = 0;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CAttribute_Button::Priority_Update(_float fTimeDelta)
{
}

void CAttribute_Button::Update(_float fTimeDelta)
{
}

void CAttribute_Button::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CAttribute_Button::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_TexNumber)))
		return E_FAIL;


	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CAttribute_Button::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Slot_Attribute"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	return S_OK;
}

CAttribute_Button* CAttribute_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAttribute_Button* pInstance = new CAttribute_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CAttribute_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAttribute_Button::Clone(void* pArg)
{
	CAttribute_Button* pInstance = new CAttribute_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CAttribute_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAttribute_Button::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
