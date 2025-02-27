#include "pch.h" 
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);

	
	/*D3D11_SAMPLER_DESC*/

	/**/
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC		EngineDesc{};

	EngineDesc.hInstance = g_hInst;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.iNumLevels = LEVEL_END;
	EngineDesc.iNumUIScenes = UISCENE_END;
	EngineDesc.isWindowed = true;
	EngineDesc.iViewportWidth = g_iWinSizeX;
	EngineDesc.iViewportHeight = g_iWinSizeY;
	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(SetUp_ImGUI(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(Start_Level(LEVEL_LOGO)))
		return E_FAIL;
	
	if (FAILED(LoadFont_Thymasia()))
		return E_FAIL;
	
	
	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);

#ifdef _DEBUG
	m_fTimerAcc += fTimeDelta;
#endif
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
}

HRESULT CMainApp::Render()
{	

	if (FAILED(m_pGameInstance->Render_Begin(_float4(0.f, 0.f, 1.f, 1.f))))
		return E_FAIL;



	m_pGameInstance->Draw();


#ifdef _DEBUG
	++m_iDrawCount;
	
	//if (1.f <= m_fTimerAcc)
	//{
	//	wsprintf(m_szFPS, TEXT("fps : %d"), m_iDrawCount);
	//	m_fTimerAcc = 0.f;
	//	m_iDrawCount = 0;
	//}

	//m_pGameInstance->Render_Font(TEXT("Font_Gulim_Default"), m_szFPS, _float2(0.f, 0.f));
	//SetWindowText(g_hWnd, m_szFPS);
	
#endif

// 3. Show another simple window.
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}


	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVELID eStartLevelID)
{
	if (LEVEL_LOADING == eStartLevelID)
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::SetUp_ImGUI(ID3D11Device* _ppDevice, ID3D11DeviceContext* _ppContext)
{
	//imgui 초기화 구문
	IMGUI_CHECKVERSION(); // 버전 체크
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	//io.DisplaySize = ImVec2(500, 500);
	
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(_ppDevice, _ppContext);

	LoadFont_ImGUI();
	

	return S_OK;
}

HRESULT CMainApp::SetUp_Window_IMGUI(HWND hWnd)
{
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(*&m_pDevice, *&m_pContext);
	return S_OK;
}

HRESULT CMainApp::Render_Window_IMGUI(HWND hWnd)
{
	return E_NOTIMPL;
}


void CMainApp::LoadFont_ImGUI()
{
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = nullptr; // imgui용 폰트
	std::string font_file = "../Bin/Font/MaruBuri-Light.ttf";

	std::ifstream ifile;
	ifile.open(font_file);
	if (ifile)
	{
		font = io.Fonts->AddFontFromFileTTF(font_file.c_str(), 16.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	}
	else
	{
		MSG_BOX("No Font file");
		font = io.Fonts->AddFontDefault();
	}
	IM_ASSERT(font != NULL);
}

HRESULT CMainApp::LoadFont_Thymasia()
{
	D3D_FEATURE_LEVEL featureLevel = m_pDevice->GetFeatureLevel();
	
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Gulim_Default"), TEXT("../Bin/Resources/Fonts/148ex.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR12"), TEXT("../Bin/Resources/Fonts/Thymesia_NotoSansKR_12.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR12_Bold"), TEXT("../Bin/Resources/Fonts/Thymesia_NotoSansKR_12Bold.spritefont"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR14"), TEXT("../Bin/Resources/Fonts/Thymesia_NotoSansKR_14.spritefont"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR16"), TEXT("../Bin/Resources/Fonts/Thymesia_NotoSansKR_16.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR18"), TEXT("../Bin/Resources/Fonts/Thymesia_NotoSansKR_18.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR24"), TEXT("../Bin/Resources/Fonts/Thymesia_NotoSansKR_24.spritefont"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR48"), TEXT("../Bin/Resources/Fonts/Thymesia_NotoSansKR_48.spritefont"))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR36"), TEXT("../Bin/Resources/Fonts/ThymasiaNotoSansKR36.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR48"), TEXT("../Bin/Resources/Fonts/ThymasiaNotoSansKR48.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR60"), TEXT("../Bin/Resources/Fonts/ThymasiaNotoSansKR60.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR72"), TEXT("../Bin/Resources/Fonts/ThymasiaNotoSansKR72.spritefont"))))
	////	return E_FAIL;



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Forum12"), TEXT("../Bin/Resources/Fonts/ThymasiaForum12.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Forum18"), TEXT("../Bin/Resources/Fonts/ThymasiaForum18.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Forum24"), TEXT("../Bin/Resources/Fonts/ThymasiaForum24.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Forum36"), TEXT("../Bin/Resources/Fonts/ThymasiaForum36.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Forum48"), TEXT("../Bin/Resources/Fonts/ThymasiaForum48.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Forum60"), TEXT("../Bin/Resources/Fonts/ThymasiaForum60.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Forum72"), TEXT("../Bin/Resources/Fonts/ThymasiaForum72.spritefont"))))
	//	return E_FAIL;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR12"), TEXT("../Bin/Resources/Fonts/ThymasiaNotoSansKR12.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR18"), TEXT("../Bin/Resources/Fonts/ThymasiaNotoSansKR18.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR24"), TEXT("../Bin/Resources/Fonts/ThymasiaNotoSansKR24.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR36"), TEXT("../Bin/Resources/Fonts/ThymasiaNotoSansKR36.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR48"), TEXT("../Bin/Resources/Fonts/ThymasiaNotoSansKR48.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR60"), TEXT("../Bin/Resources/Fonts/ThymasiaNotoSansKR60.spritefont"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NotoSansKR72"), TEXT("../Bin/Resources/Fonts/ThymasiaNotoSansKR72.spritefont"))))
	////	return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	/* 부모의 멤버를 정리하라고 이야기한다. */
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pGameInstance->Release_Engine();
	
	/*내 멤버변수를 정리한다.*/
	Safe_Release(m_pGameInstance);
}

