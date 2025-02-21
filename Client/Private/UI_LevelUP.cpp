#include "pch.h"
#include "UI_LevelUP.h"
#include "UI_Scene.h"
#include "GameInstance.h"

CUI_LevelUP::CUI_LevelUP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_LevelUP::CUI_LevelUP(const CUI_LevelUP& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_LevelUP::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_LevelUP::Initialize(void* pArg)
{
	
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	return S_OK;
}

void CUI_LevelUP::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_LevelUP::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}

void CUI_LevelUP::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI_LevelUP::Render()
{
	return S_OK;
}

HRESULT CUI_LevelUP::Ready_UIObject()
{
	LoadData_UI_Scene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP");

	/*CUIObject::UIOBJECT_DESC Desc{};
	Desc.fSizeX = 400.f;
	Desc.fSizeY = 900.f;
	Desc.fX = 300.f;
	Desc.fY = 450.f;
	m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY,TEXT("Prototype_GameObject_UI_Component"),UISCENE_LEVELUP,TEXT("UIScene_LevelUP"),CUI_Scene::UI_IMAGE,&Desc);
		
	m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY,TEXT("Prototype_GameObject_UI_Slot_Attribute"),UISCENE_LEVELUP,TEXT("UIScene_LevelUP"),CUI_Scene::UI_BUTTONPLAYER,&Desc);
		
	*/	
		
		
		//CUI_Component::UI_COMPONENT_DESC Desc{};
	//__super::Add_UI_Type(CUI_Scene::UITYPE::UI_IMAGE, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Component"), &Desc);

	//CAttribute_Button::BUTTON_DESC ButtonDesc{};
	//__super::Add_UI_Type(CUI_Scene::UITYPE::UI_BUTTONPLAYER, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Slot_Attribute"), &ButtonDesc);

	return S_OK;
}

HRESULT CUI_LevelUP::LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName)
{

	HANDLE		hFile = CreateFile(L"../Bin/DataFiles/UISave/PlayerLevelUP.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Load File", _T("Fail"), MB_OK);
		return S_OK;
	}

	DWORD	dwByte(0);
	CUIObject::UIOBJECT_DESC Desc{};
	_float3  m_fPos = {};
	_float2  m_fSize = {};
	_uint  iLen = {};
	_wstring szSaveName = {};
	_uint iUIType = {};
	_uint iShaderNum = {};

	while (true)
	{
		ReadFile(hFile, &m_fPos, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &m_fSize, sizeof(_float2), &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		szSaveName.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(szSaveName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}

		Desc.fX = m_fPos.x;
		Desc.fY = m_fPos.y;
		Desc.fZ = m_fPos.z;
		Desc.fSizeX = m_fSize.x;
		Desc.fSizeY = m_fSize.y;
		Desc.szProtoName = szSaveName;
		Desc.iShaderPassNum = iShaderNum;

		if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY, szSaveName, iSceneIndex, szSceneName, iUIType, &Desc)))
			return E_FAIL;


		//ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Load 완료", _T("성공"), MB_OK);
	return S_OK;
}

CUI_LevelUP* CUI_LevelUP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_LevelUP* pInstance = new CUI_LevelUP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_LevelUP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_LevelUP::Clone(void* pArg)
{
	CUI_LevelUP* pInstance = new CUI_LevelUP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_LevelUP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_LevelUP::Free()
{
	__super::Free();
}
