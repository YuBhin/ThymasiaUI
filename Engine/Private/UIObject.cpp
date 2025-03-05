#include "..\Public\UIObject.h"
#include "GameInstance.h"

CUIObject::CUIObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{
}

CUIObject::CUIObject(const CUIObject & Prototype)
	: CGameObject( Prototype )
	, m_ViewMatrix { Prototype.m_ViewMatrix }
	, m_ProjMatrix { Prototype.m_ProjMatrix }
{
}

HRESULT CUIObject::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixIdentity());



	return S_OK;
}

HRESULT CUIObject::Initialize(void * pArg)
{
	/* 현재 뷰포트의 정보를 얻어온다. */
	UIOBJECT_DESC*		pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	pDesc->fNear = 0.f;
	pDesc->fFar = 1.f;

	m_fPos.x = pDesc->fX;
	m_fPos.y = pDesc->fY;
	m_fPos.z = pDesc->fZ;
	m_fSize.x = pDesc->fSizeX;
	m_fSize.y = pDesc->fSizeY;

	m_strFontName = pDesc->strFontName;
	m_strContentText = pDesc->strContent;

	m_strProtoName = pDesc->strProtoName;
  	m_iShaderPassNum = pDesc->iShaderPassNum;
	m_iTexNumber = pDesc->iTexNumber;
	m_iGroupID = pDesc->iGroupID;

	_uint2			vViewportSize = m_pGameInstance->Get_ViewportSize();
	
	m_pTransformCom->Scaling(_float3(pDesc->fSizeX, pDesc->fSizeY, 1.f));

	m_pTransformCom->Rotation(XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f), pDesc->fRotation.z);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(pDesc->fX - (vViewportSize.x * 0.5f), -pDesc->fY + (vViewportSize.y * 0.5f), pDesc->fZ, 1.f));


	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(static_cast<_float>(vViewportSize.x), static_cast<_float>(vViewportSize.y), pDesc->fNear, pDesc->fFar));

	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
}

void CUIObject::Update(_float fTimeDelta)
{

}

void CUIObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CUIObject::Render()
{
	return S_OK;
}

//
//void CUIObject::Check_Render_Text()
//{
//
//	if (nullptr != m_pUITextContent)
//	{
//		_float2 TextSize = m_pGameInstance->Get_TextSize(m_pUITextFont, m_pUITextContent);
//
//		TextSize.x = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x - TextSize.x / 2;
//		TextSize.y = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y - TextSize.y / 2;
//		//{ m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x, m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y }
//		m_pGameInstance->Render_Font(TEXT("Font_Gulim_Default"), m_pUITextContent, TextSize, m_vTextClolor, 0.0f, { 0.f,0.f }, 1.f, 0.2f);
//
//	}
//
//}

HRESULT CUIObject::Add_UI_Object(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CGameObject** ppOut, void* pArg)
{
	CGameObject* pUIObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pUIObject)
		return E_FAIL;

	*ppOut = pUIObject;


	return S_OK;
}


void CUIObject::Free()
{
	__super::Free();
}
