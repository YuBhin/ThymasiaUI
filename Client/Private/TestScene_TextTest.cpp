#include "pch.h"
#include "TestScene_TextTest.h"
#include "UI_Scene.h"
#include "GameInstance.h"
#include "UI_TextBox.h"


CTestScene_TextTest::CTestScene_TextTest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CTestScene_TextTest::CTestScene_TextTest(const CTestScene_TextTest& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CTestScene_TextTest::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTestScene_TextTest::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	return S_OK;
}

void CTestScene_TextTest::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CTestScene_TextTest::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_bRenderOpen)
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_ESCMENU, L"UIScene_ESCMENU")), true);
	}
}

void CTestScene_TextTest::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_FONT, this);
}

HRESULT CTestScene_TextTest::Render()
{
	if (m_bRenderOpen)
	{
		CUI_Scene* pScene = m_pGameInstance->Find_UIScene(UISCENE_ESCMENU, L"UIScene_ESCMENU");
		vector<CUI_Text*> pTexBox =  pScene->Find_UI_TextBox();

		vector<UI_TextInfo>::iterator it;

		for (auto& TextBox : pTexBox)
		{
			_int iGroupID =  dynamic_cast<CUI_TextBox*>(TextBox)->Get_UI_GroupID();

			if (iGroupID == 1)
			{

				for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
				{
					if (it->iTextID == 1)
					{
						dynamic_cast<CUI_TextBox*>(TextBox)->Set_FontName(it->strFontName);
						dynamic_cast<CUI_TextBox*>(TextBox)->Set_ContentText(it->srtTextContent);
						break;
					}
				}
			}
			if (iGroupID == 2)
			{

				for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
				{
					if (it->iTextID == 2)
					{
						_int iHP = { 5 };
						_int iHPMAX = { 10 };
						_tchar ChangeText1[MAX_PATH] = {};

						dynamic_cast<CUI_TextBox*>(TextBox)->Set_FontName(it->strFontName);
						dynamic_cast<CUI_TextBox*>(TextBox)->Set_ContentText(it->srtTextContent);

						const _tchar* ChangeText = (*it).srtTextContent.data();


						wsprintf(ChangeText1, ChangeText, iHP, iHPMAX);

						it->srtTextContent = ChangeText1;




						break;
					}
				}
			}
		}
		
		//
		//
		//for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
		//{
		//	/*if (1 == m_iGroupID && it->iTextID == 1)
		//	{
		//		m_pGameInstance->Render_Font(it->strFontName.c_str(), it->srtTextContent.c_str(), it->fTextStartPos);

		//	}
		//	if (2 == m_iGroupID && it->iTextID == 2)
		//	{
		//		m_pGameInstance->Render_Font(it->strFontName.c_str(), it->srtTextContent.c_str(), it->fTextStartPos);

		//	}
		//	*///m_pGameInstance->Render_Font(it->strFontName.c_str(), it->srtTextContent.c_str(), it->fTextStartPos);

		//}
	}
	return S_OK;
}

HRESULT CTestScene_TextTest::Ready_UIObject()
{
	LoadData_UI_Scene(UISCENE_ESCMENU, L"UIScene_ESCMENU");
	LoadData_Text_Scene();
	return S_OK;
}

HRESULT CTestScene_TextTest::LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName)
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

HRESULT CTestScene_TextTest::LoadData_Text_Scene()
{
	HANDLE		hFile = CreateFile(L"../Bin/DataFiles/UISave/DefalutText.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

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

	/*	if (TextInfo.iTextID == 2)
		{
			_tchar ChangeText[MAX_PATH] = {};
			_int iHP = {5};
			_int iHPMAX = {10};

			wsprintf(ChangeText, TEXT("%d / %d"), iHP, iHPMAX);

			TextInfo.srtTextContent = ChangeText;
		
		}*/


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

CTestScene_TextTest* CTestScene_TextTest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTestScene_TextTest* pInstance = new CTestScene_TextTest(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTestScene_TextTest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTestScene_TextTest::Clone(void* pArg)
{
	CTestScene_TextTest* pInstance = new CTestScene_TextTest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTestScene_TextTest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTestScene_TextTest::Free()
{
	__super::Free();
}
