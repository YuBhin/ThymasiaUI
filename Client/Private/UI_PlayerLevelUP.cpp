#include "pch.h"
#include "UI_PlayerLevelUP.h"
#include "UI_Scene.h"
#include "GameInstance.h"

CUI_PlayerLevelUP::CUI_PlayerLevelUP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_PlayerLevelUP::CUI_PlayerLevelUP(const CUI_PlayerLevelUP& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_PlayerLevelUP::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PlayerLevelUP::Initialize(void* pArg)
{
	
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	return S_OK;
}

void CUI_PlayerLevelUP::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_PlayerLevelUP::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_bRenderOpen)
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), true);
	}
}

void CUI_PlayerLevelUP::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_FONT, this);
}

HRESULT CUI_PlayerLevelUP::Render()
{
	if (m_bRenderOpen)
	{
		vector<UI_TextInfo>::iterator it;
		for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
		{
			m_pGameInstance->Render_Font(it->strFontName.c_str(), it->srtTextContent.c_str(), it->fTextStartPos);

		}
	}
	return S_OK;
}

HRESULT CUI_PlayerLevelUP::Ready_UIObject()
{
	//LoadData_UI_Scene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP");

	//CUIObject::UIOBJECT_DESC Desc{};


	//if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY, L"Prototype_GameObject_UI_MouseCursor", UISCENE_LEVELUP, L"UIScene_PlayerLevelUP", UI_IMAGE, &Desc)))
	//return S_OK;

	/*CUIObject::UIOBJECT_DESC Desc{};
	Desc.fSizeX = 400.f;
	Desc.fSizeY = 900.f;
	Desc.fX = 300.f;
	Desc.fY = 450.f;
	m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY,TEXT("Prototype_GameObject_UI_Component"),UISCENE_LEVELUP,TEXT("UIScene_LevelUP"),CUI_Scene::UI_IMAGE,&Desc);
		
	m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY,TEXT("Prototype_GameObject_UI_Slot_Attribute"),UISCENE_LEVELUP,TEXT("UIScene_LevelUP"),CUI_Scene::UI_BUTTONPLAYER,&Desc);
		
	*/	
		
	LoadData_UI_Scene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP");
	LoadData_Text_Scene();
		//CUI_Component::UI_COMPONENT_DESC Desc{};
	//__super::Add_UI_Type(CUI_Scene::UITYPE::UI_IMAGE, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Component"), &Desc);

	//CAttribute_Button::BUTTON_DESC ButtonDesc{};
	//__super::Add_UI_Type(CUI_Scene::UITYPE::UI_BUTTONPLAYER, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Slot_Attribute"), &ButtonDesc);

	return S_OK;
}

HRESULT CUI_PlayerLevelUP::LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName)
{
	char   szDir[MAX_PATH] = "../Bin/DataFiles/UISave/";
	_char   szFileName[MAX_PATH] = "";

	WideCharToMultiByte(CP_ACP, 0, szSceneName, (_int)(wcslen(szSceneName)), szFileName, MAX_PATH, nullptr, nullptr);

	_char   szEXT[MAX_PATH] = ".dat";

	_char   szFullPath[MAX_PATH] = "";
	strcat_s(szFullPath, szDir);
	strcat_s(szFullPath, szFileName);
	strcat_s(szFullPath, szEXT);

	_tchar  szLastPath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, szFullPath, (_int)strlen(szFullPath), szLastPath, MAX_PATH);


	HANDLE		hFile = CreateFile(szLastPath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Load File", _T("Fail"), MB_OK);
		return S_OK;
	}

	DWORD	dwByte(0);
	CUIObject::UIOBJECT_DESC Desc{};
	_float3  fPos = {};
	_float2  fSize = {};
	_float3  fRotation = {};
	_uint  iLen = {};
	_wstring szSaveName = {};
	_uint iUIType = {};
	_uint iShaderNum = {};
	_uint iTextureNum = {};
	_uint iGroupID = {};

	while (true)
	{
		ReadFile(hFile, &fPos, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &fSize, sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &fRotation, sizeof(_float3), &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		szSaveName.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(szSaveName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);

		ReadFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iTextureNum, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iGroupID, sizeof(_uint), &dwByte, nullptr);



		if (0 == dwByte)
		{
			break;
		}

		Desc.fNear = 0.f;
		Desc.fFar = 1.f;

		Desc.fX = fPos.x;
		Desc.fY = fPos.y;
		Desc.fZ = fPos.z;
		Desc.fSizeX = fSize.x;
		Desc.fSizeY = fSize.y;
		Desc.szProtoName = szSaveName;
		Desc.iShaderPassNum = iShaderNum;
		Desc.iTexNumber = iTextureNum;
		Desc.iGroupID = iGroupID;
		Desc.fRotation = fRotation;
		if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY, szSaveName, iSceneIndex, szSceneName, iUIType, &Desc)))
			return E_FAIL;

	}
	CloseHandle(hFile);

	//MessageBox(g_hWnd, L"Load 완료", _T("성공"), MB_OK);
	return S_OK;
}

HRESULT CUI_PlayerLevelUP::LoadData_Text_Scene()
{
	HANDLE		hFile = CreateFile(L"../Bin/DataFiles/UISave/PlayerLevelUPText.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Load File - Text", _T("Fail"), MB_OK);
		return S_OK;
	}

	DWORD	dwByte(0);
	UI_TextInfo TextInfo = {};
	_uint iLen = {};

	while (true)
	{


		ReadFile(hFile, &TextInfo.iTextID, sizeof(_uint), &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		TextInfo.strFontName.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(TextInfo.strFontName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		TextInfo.srtTextContent.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(TextInfo.srtTextContent.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &TextInfo.fTextStartPos, sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &TextInfo.fTextSize, sizeof(_float2), &dwByte, nullptr);



		if (0 == dwByte)
		{
			break;
		}


		m_TextInfo.push_back(TextInfo);

	}

	CloseHandle(hFile);

	//MessageBox(g_hWnd, L"Text Load 완료", _T("성공"), MB_OK);
	return S_OK;
}

CUI_PlayerLevelUP* CUI_PlayerLevelUP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PlayerLevelUP* pInstance = new CUI_PlayerLevelUP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_PlayerLevelUP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerLevelUP::Clone(void* pArg)
{
	CUI_PlayerLevelUP* pInstance = new CUI_PlayerLevelUP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_PlayerLevelUP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerLevelUP::Free()
{
	__super::Free();
}
