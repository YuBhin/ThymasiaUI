#include "pch.h"
#include "UI_Scene_PlayerMenu.h"
#include "UI_Scene.h"
#include "GameInstance.h"

CUI_Scene_PlayerMenu::CUI_Scene_PlayerMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Scene_PlayerMenu::CUI_Scene_PlayerMenu(const CUI_Scene_PlayerMenu& Prototype)
	: CUIObject( Prototype )
{
}

HRESULT CUI_Scene_PlayerMenu::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Scene_PlayerMenu::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	return S_OK;
}

void CUI_Scene_PlayerMenu::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_Scene_PlayerMenu::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_bRenderOpen)
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), true);
	}
}

void CUI_Scene_PlayerMenu::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_FONT, this);
}

HRESULT CUI_Scene_PlayerMenu::Render()
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

HRESULT CUI_Scene_PlayerMenu::Ready_UIObject()
{
	LoadData_UI_Scene(UISCENE_MENU, L"UIScene_PlayerMenu");
	LoadData_Text_Scene();
	return S_OK;
}

HRESULT CUI_Scene_PlayerMenu::LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName)
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

	return S_OK;
}

HRESULT CUI_Scene_PlayerMenu::LoadData_Text_Scene()
{
	HANDLE		hFile = CreateFile(L"../Bin/DataFiles/UISave/PlayerMenuText.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

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

CUI_Scene_PlayerMenu* CUI_Scene_PlayerMenu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Scene_PlayerMenu* pInstance = new CUI_Scene_PlayerMenu(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_Scene_PlayerMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Scene_PlayerMenu::Clone(void* pArg)
{
	CUI_Scene_PlayerMenu* pInstance = new CUI_Scene_PlayerMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_Scene_PlayerMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Scene_PlayerMenu::Free()
{
	__super::Free();
}
