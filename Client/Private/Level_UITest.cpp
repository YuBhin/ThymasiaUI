#include "pch.h" 
#include "Level_UITest.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"	
#include "UI_Scene.h"

CLevel_UITest::CLevel_UITest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_UITest::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_UIGroup_GameIntro(TEXT("Layer_GameIntro"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_UIGroup_PlayerMenu(TEXT("Layer_PlayerMenu"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_UIGroup_PlayerLevelUP(TEXT("Layer_PlayerLevelUP"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_UIGroup_PlayerTalent(TEXT("Layer_PlayerTalent"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_UIGroup_PlayerScreen(TEXT("Layer_PlayerScreen"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_Iventory(TEXT("Layer_PlayerInventory"))))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	return S_OK;
}

void CLevel_UITest::Update(_float fTimeDelta)
{
	if (m_pGameInstance->isKeyEnter(DIK_E))
	{
		if (m_bUIOpen)
		{
			m_bUIOpen = false;
			m_pGameInstance->UIGroup_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerInventory"), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase")), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_Inven_ItemUse")), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_ItemUse_PopUp")), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_ItemType_PopUp")), false);
		}
		else
		{
			m_bUIOpen = true;
			m_pGameInstance->UIGroup_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerInventory"), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase")), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_Inven_ItemUse")), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_ItemUse_PopUp")), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_ItemType_PopUp")), true);
		}
	}

}



HRESULT CLevel_UITest::Render()
{

#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
#endif
    return S_OK;
}

HRESULT CLevel_UITest::Ready_Lights()
{
	//m_pPlayer = m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"));	

	if (FAILED(m_pGameInstance->SetUp_ShadowLight(XMVectorSet(232.f, 200.f, 150.f, 1.f), XMVectorSet(232.f, 62.f, 60.f, 1.f),
		XMConvertToRadians(60.f), g_iWinSizeX / static_cast<_float>(g_iWinSizeY), 0.1f, 800.f
		, m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player")))))
		return E_FAIL;

	LIGHT_DESC				LightDesc{};

	/* 2월 8일 빛 */
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.9f, 0.9f, 0.9f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);


	/*LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);
	LightDesc.vDiffuse   = _float4(0.9f, 0.9f, 0.9f, 1.f);
	LightDesc.vAmbient   = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vSpecular  = _float4(0.3f, 0.3f, 0.3f, 1.f);	*/

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_UITest::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_UITest::Ready_Layer_Player(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_UITest::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CCamera_Free::FREE_CAMERA_DESC		Desc = {};


	Desc.vEye = _float3(0.f, 10.f, -7.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);

	Desc.fFovy = XMConvertToRadians(60.f);
	Desc.fNear = 0.1f;
	Desc.fFar = 800.f;
	Desc.fMouseSensor = 0.05f;
	Desc.fSpeedPerSec = 25.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"), LEVEL_GAMEPLAY, pLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_UITest::Ready_Layer_UIGroup_GameIntro(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_GameIntro"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_UITest::Ready_Layer_UIGroup_PlayerMenu(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerMenu"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_UITest::Ready_Layer_UIGroup_PlayerLevelUP(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerLevelUP"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_UITest::Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerTalent"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_UITest::Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerScreen"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_UITest::Ready_Layer_UIGroup_Iventory(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_Iventory"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
CLevel_UITest* CLevel_UITest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_UITest* pInstance = new CLevel_UITest(pDevice, pContext);

if (FAILED(pInstance->Initialize()))
{
	MSG_BOX("Failed To Created : CLevel_UITest");
	Safe_Release(pInstance);
}

return pInstance;
}

void CLevel_UITest::Free()
{
	__super::Free();
}
