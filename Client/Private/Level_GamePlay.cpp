#include "pch.h" 
#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"	

#include "UI_LeftBackground.h"
#include "UI_Scene.h"
#include "UI_Button.h"
#include "UI_Button_Player.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_Text_PlayerInfo.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	Save_UI_IMGUI_Textrue();

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;
	if (!m_bIMGUI)
	{

		if (FAILED(Ready_Layer_UIGroup_GameIntro(TEXT("Layer_GameIntro"))))
			return E_FAIL;

		if (FAILED(Ready_Layer_UIGroup_PlayerMenu(TEXT("Layer_PlayerMenu"))))
			return E_FAIL;

		if (FAILED(Ready_Layer_UIGroup_PlayerLevelUP(TEXT("Layer_PlayerLevelUP"))))
			return E_FAIL;

		if (FAILED(Ready_Layer_UIGroup_PlayerTalent(TEXT("Layer_PlayerTalent"))))
			return E_FAIL;

		if (FAILED(Ready_Layer_UIGroup_PlayerScreen(TEXT("Layer_PlayerScreen"))))
			return E_FAIL;


	}
	if (FAILED(Ready_Layer_Ladder(TEXT("Layer_Ladder"))))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;


	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	if (m_bIMGUI)
	{

		if (m_pGameInstance->isMouseEnter(DIM_RB))
		{
			if (m_bUIorText)
				Create_UI_Scene_Object();
			else
				Create_Text();

		}
		if (m_pGameInstance->isKeyEnter(DIK_F4))
		{
			if (m_bUIorText)
				m_bUIorText = false;
			else
				m_bUIorText = true;

		}
		if (m_pGameInstance->isKeyEnter(DIK_F5))
		{
			if (m_bUIorText)
			{
				if (m_bGetUIObjCheck)
					m_bGetUIObjCheck = false;
				else
					m_bGetUIObjCheck = true;

			}
			else
			{
				if (m_bGetTextCheck)
					m_bGetTextCheck = false;
				else
					m_bGetTextCheck = true;

			}
		}
		if (m_pGameInstance->isKeyEnter(DIK_F6))
		{
			if (m_bMouseMoveCheck)
			{
				m_bMouseMoveCheck = false;
			}
			else
			{
				m_bMouseMoveCheck = true;
			}
			if (m_bTexMouseOn)
			{
				m_bTexMouseOn = false;
			}
			else
			{
				m_bTexMouseOn = true;
			}
		}
		if (m_pGameInstance->isKeyEnter(DIK_F8))
		{
			if (m_bUIorText)
			{
				m_pGameInstance->Clear_Choice(m_iUIType, m_iSceneIndex, m_pUISceneName, m_pCurrentUIObj);
			}
			else if(!m_bUIorText)
			{
				Delete_TextContainer_Choice();
			}
		}

	}
	else
	{

		if (m_pGameInstance->isKeyEnter(DIK_E))
		{
			m_iOpenSceneCount++;
			if (1 == m_iOpenSceneCount) // 게임 인트로
			{
				m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_GameIntro"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INTRO, L"UIScene_Intro")), true);
			}
			if (2 == m_iOpenSceneCount) // 플레이어 메뉴
			{
				m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_GameIntro"), false);
				m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerMenu"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INTRO, L"UIScene_Intro")), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), true);
			}
			if (3 == m_iOpenSceneCount) // 플레이어 레벨 업
			{
				m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerMenu"), false);
				m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerLevelUP"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), true);
			}
			if (4 == m_iOpenSceneCount) // 플레이어 특성
			{
				m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerLevelUP"), false);
				m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerTalent"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent")), true);
			}
			if (5 == m_iOpenSceneCount) // 플레이어 화면
			{
				m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerTalent"), false);
				m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerScreen"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent")), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
			}
			if (6 == m_iOpenSceneCount) // 플레이어 화면
			{
				m_iOpenSceneCount = 0;
				m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerScreen"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);
			}


		}
	}
}

HRESULT CLevel_GamePlay::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
#endif
	if (m_bIMGUI)
	{
		ShowGUI();
		//ImGui::Render();

		//m_pGameInstance->Render_Font(m_szFontName, m_szTextContent, TextTest);

	

	}
	vector<UI_TextInfo>::iterator it;
	for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
	{
		m_pGameInstance->Render_Font(it->strFontName.c_str(), it->srtTextContent.c_str(), it->fTextStartPos);

	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
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

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar* pLayerTag)
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

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar* pLayerTag)
{

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UIGroup_GameIntro(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_GameIntro"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_UIGroup_PlayerMenu(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerMenu"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_UIGroup_PlayerLevelUP(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerLevelUP"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerTalent"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerScreen"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Ladder(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Ladder"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Select_UI()
{
	/*POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_float3 vMousePos = {};

	for (auto& pUIObj : m_pGameInstance->Find_Layer_Object_All(LEVEL_GAMEPLAY, TEXT("Layer_UI_Select")))
	{

		if (ptMouse.x >= 300.f - 75.f &&
			ptMouse.x <= 300.f + 75.f &&
			ptMouse.y >= 450.f - 75.f &&
			ptMouse.y <= 450.f + 75.f)
		{
			dynamic_cast<CAttribute_Button*>(pUIObj)->Set_TexNumber(1);
		}
	}*/

	//pDesc->fSizeX = 150.f;
	//pDesc->fSizeY = 150.f;
	//pDesc->fX = 300.f;
	//pDesc->fY = 450.f;

	//vMousePos.x = ptMouse.x / (m_pGameInstance->Get_ViewportSize().x * 0.5f) - 1.f;
	//vMousePos.y = ptMouse.y / -(m_pGameInstance->Get_ViewportSize().y * 0.5f) + 1.f;
	//vMousePos.z = 0.f;

	//////투영 역행렬
	////_float4x4 ProjIverse = m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_PROJ);
	////XMStoreFloat3(&vMousePos, XMVector3TransformCoord(XMLoadFloat3(&vMousePos), XMLoadFloat4x4(&ProjIverse)));

	//_float fDist = {};

	//for (auto& pUIObj : m_pGameInstance->Find_Layer_Object_All(LEVEL_GAMEPLAY, TEXT("Layer_UI_Select")))
	//{

	//	CComponent* pVIBuffer = pUIObj->Find_Component(TEXT("Com_VIBuffer"));
	//}
	//

	return S_OK;
}

HRESULT CLevel_GamePlay::On_Mouse_Text()
{
	if (m_bGetTextCheck)
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_float2 fTextEndPos = {}; // 그려지는 텍스트 영역을 계산하기 위한 변수

		vector<UI_TextInfo>::iterator it;
		for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
		{
			fTextEndPos.x = (*it).fTextStartPos.x + (*it).fTextSize.x;
			fTextEndPos.y = (*it).fTextStartPos.y + (*it).fTextSize.y;

			if (ptMouse.x >= (*it).fTextStartPos.x &&
				ptMouse.x <= fTextEndPos.x &&
				ptMouse.y >= (*it).fTextStartPos.y &&
				ptMouse.y <= fTextEndPos.y)
			{
				m_CurrentText.iTextID = (*it).iTextID;
				m_CurrentText.strFontName = (*it).strFontName;
				m_CurrentText.srtTextContent = (*it).srtTextContent;
				m_CurrentText.fTextSize = (*it).fTextSize;
				m_CurrentText.fTextStartPos = (*it).fTextStartPos;
			}
		}
	}
	return S_OK;
}

void CLevel_GamePlay::ShowGUI()
{
	//	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Always); // 창 위치 설정
	//	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_Always); // 창 크기 설정

	//	// 방향키가 영향을 주지 않게
	//ImGui::GetIO().NavActive = false;
	//// 마우스가 영향을 주도록
	//ImGui::GetIO().WantCaptureMouse = true;

	ImGui::Begin(u8"UI Tool", NULL, ImGuiWindowFlags_MenuBar);//창 이름

	SetUIFont();
	SetUIScene();
	SetUIObj_Type();
	SetUISetting();

	//메뉴바
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			//메뉴
			if (ImGui::MenuItem("UISave"))
			{
				if (m_bUIorText)
					SaveData_UI_Scene(m_iSceneIndex, m_pUISceneName);
				else
					MessageBox(g_hWnd, L"UI 선택 후 저장하기", _T("Fail"), MB_OK);


			}
			ImGui::Separator();
			if (ImGui::MenuItem("UILoad"))
			{
				if (m_bUIorText)
					LoadData_UI_Scene(m_iSceneIndex, m_pUISceneName);
				else
					MessageBox(g_hWnd, L"UI 선택 후 불러오기", _T("Fail"), MB_OK);


			}
			//메뉴
			ImGui::Separator();
			if (ImGui::MenuItem("TextSave"))
			{
				if (!m_bUIorText)
					SaveData_Text_Scene();
				else
					MessageBox(g_hWnd, L"Text 선택 후 저장하기", _T("Fail"), MB_OK);

			}
			ImGui::Separator();
			if (ImGui::MenuItem("TextLoad"))
			{
				if (!m_bUIorText)
					LoadData_Text_Scene();
				else
					MessageBox(g_hWnd, L"Text 선택 후 불러오기", _T("Fail"), MB_OK);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Scene_Delete"))
			{
				m_pGameInstance->Clear_ALL();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Obj_Delete_Last"))
			{
				m_pGameInstance->Clear_Last(m_iUIType, m_iSceneIndex, m_pUISceneName);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Text_Delete"))
			{
				Delete_TextContainer_ALL();
			}	
			ImGui::Separator();
			if (ImGui::MenuItem("Text_Delete_Last"))
			{
				Delete_TextContainer_Last();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}


	ImGui::End();
}

void CLevel_GamePlay::SetUIScene()
{
	static _int Textchoice = 0;

	if (m_bUIorText)
		Textchoice = 0;
	else
		Textchoice = -1;


	if (ImGui::RadioButton(u8"On == UI // Off == Text _DIK_F4", Textchoice == 0))
	{
		Textchoice = 0;
	}


	static _int iradioSelect0 = 0;
	static _int iradioSelect1 = 0;

	if (ImGui::CollapsingHeader(u8"선택 상태"))
		return; // gui 숨겼다 보여주는 bool 값
	if (m_bUIorText)
	{
		if (!m_bGetUIObjCheck)
			iradioSelect0 = 0;
		else
			iradioSelect0 = -1;


		if (ImGui::RadioButton("ObjChoose_DIK_F5", iradioSelect0 == 0))
		{
			iradioSelect0 = 0;
		}

		ImGui::SameLine();

		if (m_bMouseMoveCheck)
			iradioSelect1 = 1;
		else
			iradioSelect1 = -1;
		if (ImGui::RadioButton("MouseOn_DIK_F6", iradioSelect1 == 1))
		{
			iradioSelect1 = 1;
		}

	}
	else
	{
		if (!m_bGetTextCheck)
			iradioSelect0 = 0;
		else
			iradioSelect0 = -1;


		if (ImGui::RadioButton("TextChoose_DIK_F5", iradioSelect0 == 0))
		{
			iradioSelect0 = 0;
		}

		ImGui::SameLine();

		if (m_bTexMouseOn)
			iradioSelect1 = 1;
		else
			iradioSelect1 = -1;
		if (ImGui::RadioButton("TextMouseOn_DIK_F6", iradioSelect1 == 1))
		{
			iradioSelect1 = 1;
		}
	}
	

	if (ImGui::CollapsingHeader("UIScene"))
		return; // gui 숨겼다 보여주는 bool 값
	static _int iradioSelect2 = 0;

	////초기값 설정
	//if (iradioSelect2 == 0)
	//{
	//	m_iUIScenIndex = 0;
	//	m_szUISceneName = { L"UIScene_PlayerMenu" };
	//	m_iSceneIndex = UISCENE_MENU;
	//}
	if (ImGui::RadioButton("PlayerMenu", iradioSelect2 == 0)) {
		iradioSelect2 = 0;
		m_iUIScenIndex = 0;
		m_pUISceneName = { L"UIScene_PlayerMenu" };
		m_iSceneIndex = UISCENE_MENU;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("PlayerLevelUP", iradioSelect2 == 1)) {
		iradioSelect2 = 1;
		m_iUIScenIndex = 1;
		m_pUISceneName = { L"UIScene_PlayerLevelUP" };
		m_iSceneIndex = UISCENE_LEVELUP;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("PlayerAttribute", iradioSelect2 == 2)) {
		iradioSelect2 = 2;
		m_iUIScenIndex = 2;
		m_pUISceneName = { L"UIScene_PlayerTalent" };
		m_iSceneIndex = UISCENE_TALENT;
	}
	if (ImGui::RadioButton("Intro", iradioSelect2 == 3)) {
		iradioSelect2 = 3;
		m_iUIScenIndex = 3;
		m_pUISceneName = { L"UIScene_Intro" };
		m_iSceneIndex = UISCENE_INTRO;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("ESCMENU", iradioSelect2 == 4)) {
		iradioSelect2 = 4;
		m_iUIScenIndex = 4;
		m_pUISceneName = { L"UIScene_ESCMENU" };
		m_iSceneIndex = UISCENE_ESCMENU;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("PlayerMainScreen", iradioSelect2 == 5)) {
		iradioSelect2 = 5;
		m_iUIScenIndex = 5;
		m_pUISceneName = { L"UIScene_PlayerScreen" };
		m_iSceneIndex = UISCENE_PLAYERSCREEN;
	}

	if (ImGui::RadioButton("MonsterHP", iradioSelect2 == 6)) {
		iradioSelect2 = 6;
		m_iUIScenIndex = 6;
		m_pUISceneName = { L"UIScene_MonsterHP" };
		m_iSceneIndex = UISCENE_MONSTERHP;
	}

}

void CLevel_GamePlay::SetUIObj_Type()
{
	ImGui::Text(u8"UIObjType");

	static int iradioSelect1 = 0;

	if (iradioSelect1 == 0) // 기본값 설정
	{
		m_iUIType = 0;
	}
	if (ImGui::RadioButton("UI_BUTTON", iradioSelect1 == 0)) {
		iradioSelect1 = 0;
		m_iUIType = 0;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("UI_BUTTONPLAYER", iradioSelect1 == 1)) {
		iradioSelect1 = 1;
		m_iUIType = 1;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("UI_IMAGE", iradioSelect1 == 2)) {
		iradioSelect1 = 2;
		m_iUIType = 2;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("UI_TEXT", iradioSelect1 == 3)) {
		iradioSelect1 = 3;
		m_iUIType = 3;
	}

}

void CLevel_GamePlay::SetUISetting()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	_uint2			vViewportSize = m_pGameInstance->Get_ViewportSize();

	if (0 > ptMouse.x)
	{
		ptMouse.x = (LONG)0.f;
	}
	if (1600 < ptMouse.x)
	{
		ptMouse.x = (LONG)1600.f;
	}
	if (0 > ptMouse.y)
	{
		ptMouse.y = (LONG)0.f;
	}
	if (900 < ptMouse.y)
	{
		ptMouse.y = (LONG)900.f;
	}

	if (ImGui::CollapsingHeader("UI"))
		return; // gui 숨겼다 보여주는 bool 값

	static _float3 fPos = {}; // 임구이 저장용
	static _float2 fSize = {};// 임구이 저장용
	static _float3 fRotation = {};// 임구이 저장용


	// 가져온 uiObj Pos 값
	_float3 fGetPos = {};
	_float2 fGetsize = {};
	_float3 fGetRotation = {};

	CUI_Scene* pUIScene = m_pGameInstance->Find_UIScene(m_iSceneIndex, m_pUISceneName);
	if (nullptr != pUIScene)
	{
		if (m_bGetUIObjCheck)
		{
			switch (m_iUIType)
			{
			case 0:
				for (auto& UIObj : pUIScene->Find_UI_Button())
				{
					fGetPos = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
					fGetsize.x = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().x;
					fGetsize.y = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().y;
					if (ptMouse.x >= fGetPos.x - fGetsize.x / 2 &&
						ptMouse.x <= fGetPos.x + fGetsize.x / 2 &&
						ptMouse.y >= fGetPos.y - fGetsize.y / 2 &&
						ptMouse.y <= fGetPos.y + fGetsize.y / 2)
					{
						fPos = fGetPos;
						fSize = fGetsize;
						m_pCurrentUIObj = UIObj;
					}
				}
				break;
			case 1:
				for (auto& UIObj : pUIScene->Find_UI_Button_Player())
				{
					fGetPos = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
					fGetsize.x = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().x;
					fGetsize.y = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().y;
					if (ptMouse.x >= fGetPos.x - fGetsize.x / 2 &&
						ptMouse.x <= fGetPos.x + fGetsize.x / 2 &&
						ptMouse.y >= fGetPos.y - fGetsize.y / 2 &&
						ptMouse.y <= fGetPos.y + fGetsize.y / 2)
					{
						fPos = fGetPos;
						fSize = fGetsize;
						m_pCurrentUIObj = UIObj;
					}
				}
				break;
			case 2:
				for (auto& UIObj : pUIScene->Find_UI_Image())
				{
					fGetPos = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
					fGetsize.x = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().x;
					fGetsize.y = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().y;
					if (ptMouse.x >= fGetPos.x - fGetsize.x / 2 &&
						ptMouse.x <= fGetPos.x + fGetsize.x / 2 &&
						ptMouse.y >= fGetPos.y - fGetsize.y / 2 &&
						ptMouse.y <= fGetPos.y + fGetsize.y / 2)
					{
						fPos = fGetPos;
						fSize = fGetsize;
						m_pCurrentUIObj = UIObj;
					}
				}
				break;
			case 3:
				for (auto& UIObj : pUIScene->Find_UI_TextBox())
				{
					fGetPos = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
					fGetsize.x = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().x;
					fGetsize.y = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().y;
					if (ptMouse.x >= fGetPos.x - fGetsize.x / 2 &&
						ptMouse.x <= fGetPos.x + fGetsize.x / 2 &&
						ptMouse.y >= fGetPos.y - fGetsize.y / 2 &&
						ptMouse.y <= fGetPos.y + fGetsize.y / 2)
					{
						fPos = fGetPos;
						fSize = fGetsize;
						m_pCurrentUIObj = UIObj;
					}
				}
				break;
			case 4:
				for (auto& UIObj : pUIScene->Find_UI_Text_PlayerInfo())
				{
					fGetPos = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
					fGetsize.x = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().x;
					fGetsize.y = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().y;
					if (ptMouse.x >= fGetPos.x - fGetsize.x / 2 &&
						ptMouse.x <= fGetPos.x + fGetsize.x / 2 &&
						ptMouse.y >= fGetPos.y - fGetsize.y / 2 &&
						ptMouse.y <= fGetPos.y + fGetsize.y / 2)
					{
						fPos = fGetPos;
						fSize = fGetsize;
						m_pCurrentUIObj = UIObj;
					}
				}
				break;
			}
		}
	}

	if (!m_bGetUIObjCheck) // 현재 객체가 명확하게 선택 되어 있는 경우에만 작동
	{
		if (m_bMouseMoveCheck) // 마우스 움직임 체크를 하는 경우 // 선택한 객체가 마우스를 따라갔으면 했을 때
		{
			if (nullptr != m_pCurrentUIObj)
			{
				// 객체를 마우스 따라가도록 한다
				dynamic_cast<CTransform*>(m_pCurrentUIObj->Find_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x - (vViewportSize.x * 0.5f), -fPos.y + (vViewportSize.y * 0.5f), fPos.z, 1.f));
				fPos.x = (_float)ptMouse.x;
				fPos.y = (_float)ptMouse.y;
			}
		}
	}

	ImGui::Text(u8"설치 좌표");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputFloat3("UIPos", &fPos.x, "%.2f"))
	{
		if (!m_bGetUIObjCheck)// 현재 객체가 명확하게 선택 되어 있는 경우에만 작동
		{
			if (nullptr != m_pCurrentUIObj)
			{
				dynamic_cast<CTransform*>(m_pCurrentUIObj->Find_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x - (vViewportSize.x * 0.5f), -fPos.y + (vViewportSize.y * 0.5f), fPos.z, 1.f));
				m_fPos = fPos; // Z값 저장 중
			}
		}
	}

	ImGui::Text(u8"UI 크기");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputFloat2("UISize", &fSize.x, "%.2f"))
	{
		if (!m_bGetUIObjCheck)// 현재 객체가 명확하게 선택 되어 있는 경우에만 작동
		{
			if (nullptr != m_pCurrentUIObj)
			{
				dynamic_cast<CTransform*>(m_pCurrentUIObj->Find_Component(TEXT("Com_Transform")))->Scaling(_float3(fSize.x, fSize.y, 1.f));

			}
		}
		m_fSize = fSize;
	}

	ImGui::Text(u8"UI 회전");
	ImGui::SameLine(100.f, 0.f);


	if (ImGui::InputFloat3("UIRotation", &fRotation.x, "%.2f"))
	{
		m_fRotation.x = XMConvertToRadians(fRotation.x);
		m_fRotation.y = XMConvertToRadians(fRotation.y);
		m_fRotation.z = XMConvertToRadians(fRotation.z);
	}
	if (ImGui::Button(u8"회전 적용"))
	{
		if (!m_bGetUIObjCheck)// 현재 객체가 명확하게 선택 되어 있는 경우에만 작동
		{
			dynamic_cast<CTransform*>(m_pCurrentUIObj->Find_Component(TEXT("Com_Transform")))->Rotation(XMVectorSet(0.0f,0.0f,0.1f,0.0f),m_fRotation.z);
		}
	}


	static _int iShaderPassNum = { 1 };
	ImGui::Text(u8"쉐이더 패스");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputInt(u8"패스번호", &iShaderPassNum))
	{
		m_iShaderPassNum = iShaderPassNum;
		if (!m_bGetUIObjCheck)// 현재 객체가 명확하게 선택 되어 있는 경우에만 작동
			if(nullptr != m_pCurrentUIObj)
				m_pCurrentUIObj->Set_UI_ShaderPassNum(iShaderPassNum);
	}

	static _int iTexturNum = { 0 };
	ImGui::Text(u8"텍스처번호");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputInt(u8"텍스처번호", &iTexturNum))
	{
		m_iTexNumber = iTexturNum;
		if (!m_bGetUIObjCheck)// 현재 객체가 명확하게 선택 되어 있는 경우에만 작동
			if (nullptr != m_pCurrentUIObj)
				m_pCurrentUIObj->Set_TexNumber(iTexturNum);
	}


	static _int iGroupID = { 0 };
	if (!m_bGetUIObjCheck) // 현재 객체가 명확하게 선택 되어 있는 경우에만 작동
	{
		if (nullptr != m_pCurrentUIObj)
		iGroupID = m_pCurrentUIObj->Get_UI_GroupID();
	}

	ImGui::Text(u8"UI 그룹아이디");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputInt(u8"아이디", &iGroupID))
	{
		m_iGroupID = iGroupID;
		if (!m_bGetUIObjCheck)// 현재 객체가 명확하게 선택 되어 있는 경우에만 작동
			if (nullptr != m_pCurrentUIObj)
				m_pCurrentUIObj->Set_UI_GroupID(iGroupID);
	}

	SetUIImage();
}

void CLevel_GamePlay::SetUIImage()
{
	bool ret = false;
	if (ImGui::CollapsingHeader("UIImage"))
	{
		return;
	}
	// 텍스쳐 선택
	//ImGui::Text(u8" 오브젝트들");
	_int iCount(0);
	_int iTemp(0);
	static _int iSelectButton = -1;

	ImGui::Text(u8"공용");

	map<_wstring, ID3D11ShaderResourceView*>::iterator it;
	for (it = m_ShareSRVs.begin(); it != m_ShareSRVs.end(); ++it)
	{
		ImGui::PushID(iTemp);
		ImVec4 ColorRGBA = (iSelectButton == iTemp) ? ImVec4(255.f, 0.1f, 0.0f, 0.5f) : ImVec4(0.0f, 0.f, 0.0f, 0.0f);


		_int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first.c_str(), -1, NULL, 0, NULL, NULL);
		_char* pStr = new _char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first.c_str(), -1, pStr, iStrSize, NULL, NULL);
		// 배경 그리기
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImGui::GetCursorScreenPos(),
			ImVec2(ImGui::GetCursorScreenPos().x + 64, ImGui::GetCursorScreenPos().y + 64),
			ImGui::ColorConvertFloat4ToU32(ColorRGBA));
		if (ImGui::ImageButton(pStr, (ImTextureID)(*it).second, ImVec2(64.f, 64.f)))
		{
			m_pUIName = (*it).first.c_str();
			iSelectButton = iTemp;
			//m_iNumber = iTemp;
		}

		iCount++;
		if (iCount < 5)
		{
			ImGui::SameLine();
		}
		else
		{
			iCount = 0;
		}

		delete[] pStr;
		pStr = nullptr;

		ImGui::PopID();
		iTemp++;
	}

	ImGui::NewLine();
	ImGui::Text(u8"플레이어메뉴");
	for (it = m_MenuSRVs.begin(); it != m_MenuSRVs.end(); ++it)
	{
		ImGui::PushID(iTemp);
		ImVec4 ColorRGBA = (iSelectButton == iTemp) ? ImVec4(255.f, 0.1f, 0.0f, 0.5f) : ImVec4(0.0f, 0.f, 0.0f, 0.0f);


		_int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first.c_str(), -1, NULL, 0, NULL, NULL);
		_char* pStr = new _char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first.c_str(), -1, pStr, iStrSize, NULL, NULL);
		// 배경 그리기
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImGui::GetCursorScreenPos(),
			ImVec2(ImGui::GetCursorScreenPos().x + 64, ImGui::GetCursorScreenPos().y + 64),
			ImGui::ColorConvertFloat4ToU32(ColorRGBA));
		if (ImGui::ImageButton(pStr, (ImTextureID)(*it).second, ImVec2(64.f, 64.f)))
		{
			m_pUIName = (*it).first.c_str();
			iSelectButton = iTemp;
			//m_iNumber = iTemp;
		}

		iCount++;
		if (iCount < 5)
		{
			ImGui::SameLine();
		}
		else
		{
			iCount = 0;
		}

		delete[] pStr;
		pStr = nullptr;

		ImGui::PopID();
		iTemp++;
	}
	ImGui::NewLine();

	ImGui::Text(u8"플레이어레벨업");
	for (it = m_LevelUpSRVs.begin(); it != m_LevelUpSRVs.end(); ++it)
	{
		ImGui::PushID(iTemp);
		ImVec4 ColorRGBA = (iSelectButton == iTemp) ? ImVec4(255.f, 0.1f, 0.0f, 0.5f) : ImVec4(0.0f, 0.f, 0.0f, 0.0f);


		_int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first.c_str(), -1, NULL, 0, NULL, NULL);
		_char* pStr = new _char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first.c_str(), -1, pStr, iStrSize, NULL, NULL);
		// 배경 그리기
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImGui::GetCursorScreenPos(),
			ImVec2(ImGui::GetCursorScreenPos().x + 64, ImGui::GetCursorScreenPos().y + 64),
			ImGui::ColorConvertFloat4ToU32(ColorRGBA));
		if (ImGui::ImageButton(pStr, (ImTextureID)(*it).second, ImVec2(64.f, 64.f)))
		{
			m_pUIName = (*it).first.c_str();
			iSelectButton = iTemp;
			//m_iNumber = iTemp;
		}

		iCount++;
		if (iCount < 5)
		{
			ImGui::SameLine();
		}
		else
		{
			iCount = 0;
		}

		delete[] pStr;
		pStr = nullptr;

		ImGui::PopID();
		iTemp++;
	}
	ImGui::NewLine();

	ImGui::Text(u8"플레이어특성");
	for (it = m_AttributeSRVs.begin(); it != m_AttributeSRVs.end(); ++it)
	{
		ImGui::PushID(iTemp);
		ImVec4 ColorRGBA = (iSelectButton == iTemp) ? ImVec4(255.f, 0.1f, 0.0f, 0.5f) : ImVec4(0.0f, 0.f, 0.0f, 0.0f);


		_int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first.c_str(), -1, NULL, 0, NULL, NULL);
		_char* pStr = new _char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first.c_str(), -1, pStr, iStrSize, NULL, NULL);
		// 배경 그리기
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImGui::GetCursorScreenPos(),
			ImVec2(ImGui::GetCursorScreenPos().x + 64, ImGui::GetCursorScreenPos().y + 64),
			ImGui::ColorConvertFloat4ToU32(ColorRGBA));
		if (ImGui::ImageButton(pStr, (ImTextureID)(*it).second, ImVec2(64.f, 64.f)))
		{
			m_pUIName = (*it).first.c_str();
			iSelectButton = iTemp;
			//m_iNumber = iTemp;
		}

		iCount++;
		if (iCount < 5)
		{
			ImGui::SameLine();
		}
		else
		{
			iCount = 0;
		}

		delete[] pStr;
		pStr = nullptr;

		ImGui::PopID();
		iTemp++;
	}

	ImGui::NewLine();
	
	ImGui::Text(u8"플레이어 기본화면");
	for (it = m_PlayerSRVs.begin(); it != m_PlayerSRVs.end(); ++it)
	{
		ImGui::PushID(iTemp);
		ImVec4 ColorRGBA = (iSelectButton == iTemp) ? ImVec4(255.f, 0.1f, 0.0f, 0.5f) : ImVec4(0.0f, 0.f, 0.0f, 0.0f);


		_int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first.c_str(), -1, NULL, 0, NULL, NULL);
		_char* pStr = new _char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first.c_str(), -1, pStr, iStrSize, NULL, NULL);
		// 배경 그리기
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImGui::GetCursorScreenPos(),
			ImVec2(ImGui::GetCursorScreenPos().x + 64, ImGui::GetCursorScreenPos().y + 64),
			ImGui::ColorConvertFloat4ToU32(ColorRGBA));
		if (ImGui::ImageButton(pStr, (ImTextureID)(*it).second, ImVec2(64.f, 64.f)))
		{
			m_pUIName = (*it).first.c_str();
			iSelectButton = iTemp;
			//m_iNumber = iTemp;
		}

		iCount++;
		if (iCount < 5)
		{
			ImGui::SameLine();
		}
		else
		{
			iCount = 0;
		}

		delete[] pStr;
		pStr = nullptr;

		ImGui::PopID();
		iTemp++;
	}

	ImGui::NewLine();

}

void CLevel_GamePlay::SetUIFont()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	static _float2 fTextPos = {};
	vector<UI_TextInfo>::iterator it;
	On_Mouse_Text();

	if (ImGui::CollapsingHeader("Font"))
	{
		return;
	}
	ImGui::Separator();
	SetUIFontName();
	ImGui::Separator();

	ImGui::Text(u8"FontInfo");
	ImGui::Separator();

	//ImGui::Text("Score: %d", score);              // 정수 출력
	//ImGui::Text("Time Elapsed: %.2f seconds", timeElapsed); // 실수 출력 (소수점 2자리)
	//ImGui::Text("Message: %s", text.c_str()); // 문자열 출력

	ImGui::BulletText("TextID: %d", m_CurrentText.iTextID);
	ImGui::BulletText(u8"사용 폰트: %s", m_CurrentText.strFontName.data());
	ImGui::BulletText(u8"텍스트 내용: %s", m_CurrentText.srtTextContent.data());
	ImGui::BulletText(u8"가로 세로: %.2f / %.2f", m_CurrentText.fTextSize.x, m_CurrentText.fTextSize.y);
	ImGui::BulletText("X Y : %.2f / %.2f", m_CurrentText.fTextStartPos.x, m_CurrentText.fTextStartPos.y);
	ImGui::Separator();


	ImGui::Text(u8"Font Text 입력");
	static _char szInputText[MAX_PATH] = "";
	 
	if (ImGui::InputTextMultiline(u8"텍스트 내용 작성", szInputText, IM_ARRAYSIZE(szInputText),ImVec2(300.f,100.f)))
	{
		if (nullptr != m_pSaveName)
		{
			delete[] m_pSaveName;
		}

		_int iResize = MultiByteToWideChar(CP_UTF8, 0, szInputText, -1, NULL, 0);
		m_pSaveName = new _tchar[iResize];

		MultiByteToWideChar(CP_UTF8, 0, szInputText, -1, m_pSaveName, iResize);

		m_pTextContent = m_pSaveName;
		//delete[] m_szSaveName;

		// 디버깅용 출력
	}
	else
		m_bCurrentText = false;
	ImGui::Text(u8"작성된: %s", szInputText);


	if (ImGui::Button(u8"텍스트 적용"))
	{
		if (!m_bGetUIObjCheck)// 현재 객체가 명확하게 선택 되어 있는 경우에만 작동
		{
			m_pTextBoxFont = m_pFontName;
			m_pTextBoxContent = m_pSaveName;
			m_pCurrentUIObj->Set_FontName(m_pTextBoxFont);
			m_pCurrentUIObj->Set_Content(m_pTextBoxContent);
		}
	}


	static _int iTextID = {};

	ImGui::Text(u8"텍스트ID");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputInt("TextID", &iTextID))
	{
		m_iTextID = iTextID;
	}



	if (!m_bGetTextCheck)// 현재 객체가 명확하게 선택 되어 있는 경우에만 작동
	{
		if (m_bTexMouseOn) // 마우스 켜져 있음
		{
			for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
			{
				if ((*it).iTextID == m_CurrentText.iTextID)
				{
					// 마우스 좌표를 텍스트 그려지는 좌표로 계산을 해서 집어 넣는다
					(*it).fTextStartPos.x = ptMouse.x - (*it).fTextSize.x / 2;
					(*it).fTextStartPos.y = ptMouse.y - (*it).fTextSize.y / 2;
				}
			}
		}
	}
	// 텍스트의 중점을 Imgui에 표시한다
	fTextPos.x = m_CurrentText.fTextStartPos.x + m_CurrentText.fTextSize.x / 2;
	fTextPos.y = m_CurrentText.fTextStartPos.y + m_CurrentText.fTextSize.y / 2;

	ImGui::Text(u8"텍스트 중점 좌표");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputFloat2("TextPos", &fTextPos.x, "%.2f"))
	{
		if (!m_bGetTextCheck)// 현재 객체가 명확하게 선택 되어 있는 경우에만 작동
		{
			if (!m_bTexMouseOn) // 마우스 꺼져있음
			{
				for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
				{
					if ((*it).iTextID == m_CurrentText.iTextID)
					{
						(*it).fTextStartPos.x = fTextPos.x - (*it).fTextSize.x / 2;
						(*it).fTextStartPos.y = fTextPos.y - (*it).fTextSize.y / 2;
					}
				}
			}
		}
	}


	ImGui::Separator();
}

void CLevel_GamePlay::SetUIFontName()
{
	static int iradioSelect1 = 0;
	
		if (ImGui::RadioButton("Font_Gulim_Default", iradioSelect1 == 0)) {
			iradioSelect1 = 0;
			m_pFontName = TEXT("Font_Gulim_Default");
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("NotoSansKR12", iradioSelect1 == 1)) {
			iradioSelect1 = 1;
			m_pFontName = TEXT("Font_NotoSansKR12");
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("NotoSansKR12_Bold", iradioSelect1 == 2)) {
			iradioSelect1 = 2;
			m_pFontName = TEXT("Font_NotoSansKR12_Bold");
		}
		if (ImGui::RadioButton("NotoSansKR14", iradioSelect1 == 3)) {
			iradioSelect1 = 3;
			m_pFontName = TEXT("Font_NotoSansKR14");
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("NotoSansKR16", iradioSelect1 == 4)) {
			iradioSelect1 = 4;
			m_pFontName = TEXT("Font_NotoSansKR16");
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("NotoSansKR18", iradioSelect1 == 5)) {
			iradioSelect1 = 5;
			m_pFontName = TEXT("Font_NotoSansKR18");
		}
		if (ImGui::RadioButton("NotoSansKR24", iradioSelect1 == 6)) {
			iradioSelect1 = 6;
			m_pFontName = TEXT("Font_NotoSansKR24");
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("NotoSansKR48", iradioSelect1 == 7)) {
			iradioSelect1 = 7;
			m_pFontName = TEXT("Font_NotoSansKR48");
		}


}

HRESULT CLevel_GamePlay::Create_UI_Scene_Object()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	CUIObject::UIOBJECT_DESC Desc{};

	if (nullptr == m_pUIName)
	{
		MessageBox(g_hWnd, L"생성할 UI 선택 필요", _T("Fail"), MB_OK);
		return E_FAIL;

	}

	Desc.fSizeX = m_fSize.x;
	Desc.fSizeY = m_fSize.y;
	Desc.fX = (_float)ptMouse.x;
	Desc.fY = (_float)ptMouse.y;
	Desc.fZ = m_fPos.z;
	Desc.strFontName = m_pTextBoxFont;
	Desc.strContent = m_pTextBoxContent;
	Desc.strProtoName = m_pUIName;
	Desc.iShaderPassNum = m_iShaderPassNum;
	Desc.iTexNumber = m_iTexNumber;
	Desc.iGroupID = m_iGroupID;
	Desc.fRotation = m_fRotation;
	m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY, m_pUIName, m_iSceneIndex, m_pUISceneName, m_iUIType, &Desc);



	return S_OK;
}

HRESULT CLevel_GamePlay::Save_UI_IMGUI_Textrue()
{
	//====================================================================================================================================== 키보드 텍스처
	Save_UI_Textrue_Share(TEXT("Prototype_GameObject_UI_KeyBox_Long"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_KeyBox_Long.dds"), 1);
	Save_UI_Textrue_Share(TEXT("Prototype_GameObject_UI_TextBox"), TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_TextBox.dds"), 1);

	//====================================================================================================================================== 넛지 텍스처
	Save_UI_Textrue_Share(TEXT("Prototype_GameObject_UI_DiamondIcon"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_DiamondIcon_01.dds"), 1);
	Save_UI_Textrue_Share(TEXT("Prototype_GameObject_UI_Indicator_Selected"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_Indicator_Selected_0.dds"), 1);
	Save_UI_Textrue_Share(TEXT("Prototype_GameObject_UI_NewHint"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_NewHint.dds"), 1);
	Save_UI_Textrue_Share(TEXT("Prototype_GameObject_UI_LockedIcon"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_LockedIcon.dds"), 1);

	//==================================================================================================================================== 플레이어 메뉴창(의자착석)
	Save_UI_Textrue_Menu(TEXT("Prototype_GameObject_UI_LeftBackground"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_LeftBackground.dds"), 1);
	Save_UI_Textrue_Menu(TEXT("Prototype_GameObject_UI_RightBackground"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_RightBackground.dds"), 1);
	Save_UI_Textrue_Menu(TEXT("Prototype_GameObject_UI_HighlightBar"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_HighlightBar_00.dds"), 1);
	Save_UI_Textrue_Menu(TEXT("Prototype_GameObject_UI_LevelImage"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_LevelImage_Circus.dds"), 1);
	Save_UI_Textrue_Menu(TEXT("Prototype_GameObject_UI_ButtonHighlight"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_ButtonHighlight_00.dds"), 1);
	//==================================================================================================================================== 플레이어 특성 창

	Save_UI_Textrue_Attribute(TEXT("Prototype_GameObject_UI_Arrow"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_Arrow.dds"), 1);
	Save_UI_Textrue_Attribute(TEXT("Prototype_GameObject_UI_Attribute_Slot_Active"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_Attribute_Slot_Active_0.dds"), 1);
	Save_UI_Textrue_Attribute(TEXT("Prototype_GameObject_UI_BaseTalentIndicator"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_BaseTalentIndicator.dds"), 1);
	Save_UI_Textrue_Attribute(TEXT("Prototype_GameObject_UI_ContentBackground"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_ContentBackground.dds"), 1);
	Save_UI_Textrue_Attribute(TEXT("Prototype_GameObject_UI_DescBackground"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_DescBackground.dds"), 1);
	Save_UI_Textrue_Attribute(TEXT("Prototype_GameObject_UI_Frame"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_Frame_0.dds"), 1);
	Save_UI_Textrue_Attribute(TEXT("Prototype_GameObject_UI_SceneAboveBackground"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_SceneAboveBackground.dds"), 1);
	Save_UI_Textrue_Attribute(TEXT("Prototype_GameObject_UI_SceneBackground"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_SceneBackground_0.dds"), 1);
	Save_UI_Textrue_Attribute(TEXT("Prototype_GameObject_UI_SceneUnderBackground"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_SceneUnderBackground.dds"), 1);
	Save_UI_Textrue_Attribute(TEXT("Prototype_GameObject_UI_UnderLine"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_UnderLine.dds"), 1);
	//==================================================================================================================================== 플레이어 레벨 업 창

	Save_UI_Textrue_LevelUp(TEXT("Prototype_GameObject_UI_Arrow_Long"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerLevelUP/UI_Arrow_Long.dds"), 1);
	Save_UI_Textrue_LevelUp(TEXT("Prototype_GameObject_UI_BlackBar"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerLevelUP/UI_BlackBar.dds"), 1);
	Save_UI_Textrue_LevelUp(TEXT("Prototype_GameObject_UI_DecorationFrame"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerLevelUP/UI_DecorationFrame_01.dds"), 1);
	Save_UI_Textrue_LevelUp(TEXT("Prototype_GameObject_UI_ButtonBackground"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerLevelUP/UI_ButtonBackground.dds"), 1);
	Save_UI_Textrue_LevelUp(TEXT("Prototype_GameObject_UI_TitleBackgroundBar"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerLevelUP/UI_TitleBackgroundBar_00.dds"), 1);

	//====================================================================================================================================== 플레이어 기본화면 
	
	Save_UI_Textrue_PlayerMainScreen(TEXT("Prototype_GameObject_UI_Feather_Icon"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_Feather_Icon.dds"), 1);
	Save_UI_Textrue_PlayerMainScreen(TEXT("Prototype_GameObject_UI_HPBar1_Border"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_HPBar1_Border.dds"), 1);
	Save_UI_Textrue_PlayerMainScreen(TEXT("Prototype_GameObject_UI_HPBar2_BG"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_HPBar2_BG.dds"), 1);
	Save_UI_Textrue_PlayerMainScreen(TEXT("Prototype_GameObject_UI_HPBar3_MainBar"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_HPBar3_MainBar.dds"), 1);
	Save_UI_Textrue_PlayerMainScreen(TEXT("Prototype_GameObject_UI_MemoryShardsIcon"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_MemoryShardsIcon.dds"), 1);
	Save_UI_Textrue_PlayerMainScreen(TEXT("Prototype_GameObject_UI_MPBar1_Border"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_MPBar1_Border.dds"), 1);
	Save_UI_Textrue_PlayerMainScreen(TEXT("Prototype_GameObject_UI_MPBar2_BG"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_MPBar2_BG.dds"), 1);
	Save_UI_Textrue_PlayerMainScreen(TEXT("Prototype_GameObject_UI_MPBar3_MainBar"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_MPBar3_MainBar.dds"), 1);
	Save_UI_Textrue_PlayerMainScreen(TEXT("Prototype_GameObject_UI_PlunderSlotFrame"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_PlunderSlotFrame_0.dds"), 1);
	Save_UI_Textrue_PlayerMainScreen(TEXT("Prototype_GameObject_UI_Potion_DefaultType"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_Potion_DefaultType.dds"), 1);

	//====================================================================================================================================== 공용 꾸밈 텍스처
	Save_UI_Textrue_Share(TEXT("Prototype_GameObject_UI_DecorationLine"), TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_DecorationLine_0.dds"), 1);
	Save_UI_Textrue_Share(TEXT("Prototype_GameObject_UI_GameLogoImage"), TEXT("../Bin/Resources/Textures/ThymesiaUI/GameIntro/UI_GameLogoImage.dds"), 1);

	return S_OK;
}

HRESULT CLevel_GamePlay::Save_UI_Textrue_Share(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	m_iCountSRVs += iNumTextures;

	_tchar		szName[MAX_PATH] = {};
	_tchar		szEXT[MAX_PATH] = {};

	_wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, szName, MAX_PATH, szEXT, MAX_PATH);

	for (size_t i = 0; i < iNumTextures; i++)
	{
		ID3D11Texture2D* pTexture2D = { nullptr };
		ID3D11ShaderResourceView* pSRV = { nullptr };

		_tchar			szTextureFilePath[MAX_PATH] = TEXT("");

		wsprintf(szTextureFilePath, pTextureFilePath, i);
		wsprintf(szName, szName, i);

		HRESULT			hr = {};


		if (false == lstrcmp(szEXT, TEXT(".dds")))
		{
			/* .dds */
			hr = CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}
		else if (false == lstrcmp(szEXT, TEXT(".tga")))
		{
			hr = E_FAIL;
		}
		else
		{	/* .jpg etc */
			hr = CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}

		if (FAILED(hr))
			return E_FAIL;

		m_ShareSRVs.emplace(_pObjTag, pSRV);
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Save_UI_Textrue_Menu(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	m_iCountSRVs += iNumTextures;

	_tchar		szName[MAX_PATH] = {};
	_tchar		szEXT[MAX_PATH] = {};

	_wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, szName, MAX_PATH, szEXT, MAX_PATH);

	for (size_t i = 0; i < iNumTextures; i++)
	{
		ID3D11Texture2D* pTexture2D = { nullptr };
		ID3D11ShaderResourceView* pSRV = { nullptr };

		_tchar			szTextureFilePath[MAX_PATH] = TEXT("");

		wsprintf(szTextureFilePath, pTextureFilePath, i);
		wsprintf(szName, szName, i);

		HRESULT			hr = {};


		if (false == lstrcmp(szEXT, TEXT(".dds")))
		{
			/* .dds */
			hr = CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}
		else if (false == lstrcmp(szEXT, TEXT(".tga")))
		{
			hr = E_FAIL;
		}
		else
		{	/* .jpg etc */
			hr = CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}

		if (FAILED(hr))
			return E_FAIL;

		m_MenuSRVs.emplace(_pObjTag, pSRV);
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Save_UI_Textrue_LevelUp(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	m_iCountSRVs += iNumTextures;

	_tchar		szName[MAX_PATH] = {};
	_tchar		szEXT[MAX_PATH] = {};

	_wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, szName, MAX_PATH, szEXT, MAX_PATH);

	for (size_t i = 0; i < iNumTextures; i++)
	{
		ID3D11Texture2D* pTexture2D = { nullptr };
		ID3D11ShaderResourceView* pSRV = { nullptr };

		_tchar			szTextureFilePath[MAX_PATH] = TEXT("");

		wsprintf(szTextureFilePath, pTextureFilePath, i);
		wsprintf(szName, szName, i);

		HRESULT			hr = {};


		if (false == lstrcmp(szEXT, TEXT(".dds")))
		{
			/* .dds */
			hr = CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}
		else if (false == lstrcmp(szEXT, TEXT(".tga")))
		{
			hr = E_FAIL;
		}
		else
		{	/* .jpg etc */
			hr = CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}

		if (FAILED(hr))
			return E_FAIL;

		m_LevelUpSRVs.emplace(_pObjTag, pSRV);
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Save_UI_Textrue_Attribute(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	m_iCountSRVs += iNumTextures;

	_tchar		szName[MAX_PATH] = {};
	_tchar		szEXT[MAX_PATH] = {};

	_wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, szName, MAX_PATH, szEXT, MAX_PATH);

	for (size_t i = 0; i < iNumTextures; i++)
	{
		ID3D11Texture2D* pTexture2D = { nullptr };
		ID3D11ShaderResourceView* pSRV = { nullptr };

		_tchar			szTextureFilePath[MAX_PATH] = TEXT("");

		wsprintf(szTextureFilePath, pTextureFilePath, i);
		wsprintf(szName, szName, i);

		HRESULT			hr = {};


		if (false == lstrcmp(szEXT, TEXT(".dds")))
		{
			/* .dds */
			hr = CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}
		else if (false == lstrcmp(szEXT, TEXT(".tga")))
		{
			hr = E_FAIL;
		}
		else
		{	/* .jpg etc */
			hr = CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}

		if (FAILED(hr))
			return E_FAIL;

		m_AttributeSRVs.emplace(_pObjTag, pSRV);
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Save_UI_Textrue_PlayerMainScreen(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	m_iCountSRVs += iNumTextures;

	_tchar		szName[MAX_PATH] = {};
	_tchar		szEXT[MAX_PATH] = {};

	_wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, szName, MAX_PATH, szEXT, MAX_PATH);

	for (size_t i = 0; i < iNumTextures; i++)
	{
		ID3D11Texture2D* pTexture2D = { nullptr };
		ID3D11ShaderResourceView* pSRV = { nullptr };

		_tchar			szTextureFilePath[MAX_PATH] = TEXT("");

		wsprintf(szTextureFilePath, pTextureFilePath, i);
		wsprintf(szName, szName, i);

		HRESULT			hr = {};


		if (false == lstrcmp(szEXT, TEXT(".dds")))
		{
			/* .dds */
			hr = CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}
		else if (false == lstrcmp(szEXT, TEXT(".tga")))
		{
			hr = E_FAIL;
		}
		else
		{	/* .jpg etc */
			hr = CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}

		if (FAILED(hr))
			return E_FAIL;

		m_PlayerSRVs.emplace(_pObjTag, pSRV);
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::SaveData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName) // 파라미터 => Load 시 각 씬 클래스 에서 집어 넣도록 = 여기서는 imgui를 통해서
{
	_char   szDir[MAX_PATH] = "../Bin/DataFiles/UISave/";
	_char   szFileName[MAX_PATH] = "";

	WideCharToMultiByte(CP_ACP, 0, szSceneName, (_int)(wcslen(szSceneName)), szFileName, MAX_PATH, nullptr, nullptr);

	_char   szEXT[MAX_PATH] = ".dat";

	_char   szFullPath[MAX_PATH] = "";
	strcat_s(szFullPath, szDir);
	strcat_s(szFullPath, szFileName);
	strcat_s(szFullPath, szEXT);

	_tchar  szLastPath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, szFullPath, (_int)strlen(szFullPath), szLastPath, MAX_PATH);


	//const _tchar* szUISceneName = L"../Bin/DataFiles/UISave/Defalut.dat";
	//if (0 == wcscmp(szSceneName, L"UIScene_PlayerMenu"))
	//	szUISceneName = L"../Bin/DataFiles/UISave/PlayerMenu.dat";
	//if (0 == wcscmp(szSceneName, L"UIScene_PlayerLevelUP"))
	//	szUISceneName = L"../Bin/DataFiles/UISave/PlayerLevelUP.dat";
	//if (0 == wcscmp(szSceneName, L"UIScene_PlayerAttribute"))
	//	szUISceneName = L"../Bin/DataFiles/UISave/PlayerAttribute.dat";

	HANDLE		hFile = CreateFile(szLastPath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Save File - UIScene", _T("Fail"), MB_OK);
		return S_OK;
	}

	DWORD	dwByte(0);
	_uint iUIType = {};
	_uint iLen = {};
	_uint iShaderNum = {};
	_uint iTextureNum = {};
	_uint iGroupID = {};
	_float3 fPos = {};
	_float2 fScale = {};
	_float3 fRotation = {};

	for (auto& Button : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_Button())
	{
		iUIType = UI_BUTTON;

		fPos = dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
		fScale.x = dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().x;
		fScale.y = dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().y;
		fRotation = dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Get_Rotation_UIObj();

		WriteFile(hFile, &fPos, sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &fScale,sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &fRotation, sizeof(_float3), &dwByte, nullptr);

		iLen = (_uint)Button->Get_UI_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, Button->Get_UI_Name().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);

		WriteFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);

		iShaderNum = Button->Get_UI_ShaderPassNum();
		WriteFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);
		
		iTextureNum = Button->Get_UI_TexNumber();
		WriteFile(hFile, &iTextureNum, sizeof(_uint), &dwByte, nullptr);
		
		iGroupID = Button->Get_UI_GroupID();
		WriteFile(hFile, &iGroupID, sizeof(_uint), &dwByte, nullptr);
	}

	for (auto& Button_Player : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_Button_Player())
	{
		iUIType = UI_BUTTONPLAYER;


		fPos = dynamic_cast<CTransform*>(Button_Player->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
		fScale.x = dynamic_cast<CTransform*>(Button_Player->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().x;
		fScale.y = dynamic_cast<CTransform*>(Button_Player->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().y;
		fRotation = dynamic_cast<CTransform*>(Button_Player->Find_Component(TEXT("Com_Transform")))->Get_Rotation_UIObj();

		WriteFile(hFile, &fPos, sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &fScale, sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &fRotation, sizeof(_float3), &dwByte, nullptr);

		iLen = (_uint)Button_Player->Get_UI_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, Button_Player->Get_UI_Name().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);

		WriteFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);

		iShaderNum = Button_Player->Get_UI_ShaderPassNum();
		WriteFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);

		iTextureNum = Button_Player->Get_UI_TexNumber();
		WriteFile(hFile, &iTextureNum, sizeof(_uint), &dwByte, nullptr);

		iGroupID = Button_Player->Get_UI_GroupID();
		WriteFile(hFile, &iGroupID, sizeof(_uint), &dwByte, nullptr);
	}

	for (auto& Image : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_Image())
	{
 		iUIType = UI_IMAGE;


		fPos = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
		fScale.x = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().x;
		fScale.y = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().y;
		fRotation = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_Rotation_UIObj();
		
		WriteFile(hFile, &fPos, sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &fScale, sizeof(_float2), &dwByte, nullptr);
 		WriteFile(hFile, &fRotation, sizeof(_float3), &dwByte, nullptr);

		iLen = (_uint)Image->Get_UI_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, Image->Get_UI_Name().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);

		WriteFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);

		iShaderNum = Image->Get_UI_ShaderPassNum();
		WriteFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);

		iTextureNum = Image->Get_UI_TexNumber();
		WriteFile(hFile, &iTextureNum, sizeof(_uint), &dwByte, nullptr);

		iGroupID = Image->Get_UI_GroupID();
		WriteFile(hFile, &iGroupID, sizeof(_uint), &dwByte, nullptr);
	}
	
	for (auto& TextBox : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_TextBox())
	{
 		iUIType = UI_TEXT;

		fPos = dynamic_cast<CTransform*>(TextBox->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
		fScale.x = dynamic_cast<CTransform*>(TextBox->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().x;
		fScale.y = dynamic_cast<CTransform*>(TextBox->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().y;
		fRotation = dynamic_cast<CTransform*>(TextBox->Find_Component(TEXT("Com_Transform")))->Get_Rotation_UIObj();
		
		WriteFile(hFile, &fPos, sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &fScale, sizeof(_float2), &dwByte, nullptr);
 		WriteFile(hFile, &fRotation, sizeof(_float3), &dwByte, nullptr);

		iLen = (_uint)TextBox->Get_UI_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, TextBox->Get_UI_Name().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);

		WriteFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);

		iLen = (_uint)TextBox->Get_Font_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, TextBox->Get_Font_Name().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);
		
		iLen = (_uint)TextBox->Get_Content().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, TextBox->Get_Content().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);

		iShaderNum = TextBox->Get_UI_ShaderPassNum();
		WriteFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);

		iTextureNum = TextBox->Get_UI_TexNumber();
		WriteFile(hFile, &iTextureNum, sizeof(_uint), &dwByte, nullptr);

		iGroupID = TextBox->Get_UI_GroupID();
		WriteFile(hFile, &iGroupID, sizeof(_uint), &dwByte, nullptr);
	}

	for (auto& Text_PlayerInfo : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_Text_PlayerInfo())
	{
		iUIType = UI_TEXTPLAYER;

		WriteFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);

		fPos = dynamic_cast<CTransform*>(Text_PlayerInfo->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
		fScale.x = dynamic_cast<CTransform*>(Text_PlayerInfo->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().x;
		fScale.y = dynamic_cast<CTransform*>(Text_PlayerInfo->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().y;
		fRotation = dynamic_cast<CTransform*>(Text_PlayerInfo->Find_Component(TEXT("Com_Transform")))->Get_Rotation_UIObj();

		WriteFile(hFile, &fPos, sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &fScale, sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &fRotation, sizeof(_float3), &dwByte, nullptr);

		iLen = (_uint)Text_PlayerInfo->Get_UI_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, Text_PlayerInfo->Get_UI_Name().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);


		iShaderNum = Text_PlayerInfo->Get_UI_ShaderPassNum();
		WriteFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);

		iTextureNum = Text_PlayerInfo->Get_UI_TexNumber();
		WriteFile(hFile, &iTextureNum, sizeof(_uint), &dwByte, nullptr);

		iGroupID = Text_PlayerInfo->Get_UI_GroupID();
		WriteFile(hFile, &iGroupID, sizeof(_uint), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Save 완료", _T("성공"), MB_OK);
	return S_OK;
}

HRESULT CLevel_GamePlay::LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName)
{
	m_pGameInstance->Clear_ALL();

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
		MessageBox(g_hWnd, szLastPath, TEXT("Fail"), MB_OK);
		return S_OK;
	}

	DWORD	dwByte(0);
 	CUIObject::UIOBJECT_DESC Desc{};
	_float3  fPos = {};
	_float2  fSize = {};
	_float3  fRotation = {};
	_uint  iLen = {};
	_wstring szFontName = {};
	_wstring szContentText = {};
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
		if (iUIType == UI_TEXT)
		{
			ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
			szFontName.resize(iLen);
			ReadFile(hFile, const_cast<wchar_t*>(szFontName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

			ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
			szContentText.resize(iLen);
			ReadFile(hFile, const_cast<wchar_t*>(szContentText.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		}

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

		Desc.strFontName = szFontName;
		Desc.strContent = szContentText;

		Desc.strProtoName = szSaveName;
		Desc.iShaderPassNum = iShaderNum;
		Desc.iTexNumber = iTextureNum;
		Desc.iGroupID = iGroupID;
		Desc.fRotation = fRotation;
		if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY, szSaveName, iSceneIndex, szSceneName, iUIType, &Desc)))
			return E_FAIL;

	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Load 완료", _T("성공"), MB_OK);
	return S_OK;
}

HRESULT CLevel_GamePlay::Create_Text()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	UI_TextInfo Info = {};

	_float2 TextSize = m_pGameInstance->Get_TextSize(m_pFontName, m_pTextContent);//텍스트 가로 세로 길이
	Info.fTextSize = TextSize;

	// 소환하는 좌표와 그려지는 텍스트의 중점을 맞추는 계산
	TextSize.x = ptMouse.x - TextSize.x / 2;
	TextSize.y = ptMouse.y - TextSize.y / 2;
	Info.fTextStartPos = TextSize;

	Info.strFontName = m_pFontName;
	Info.srtTextContent = m_pTextContent;
	Info.iTextID = m_iTextID;

	m_TextInfo.push_back(Info);

	return S_OK;
}

HRESULT CLevel_GamePlay::SaveData_Text_Scene()
{
	char   szDir[MAX_PATH] = "../Bin/DataFiles/UISave/";
	_char   szFileName[MAX_PATH] = "";

	WideCharToMultiByte(CP_ACP, 0, m_pUISceneName, (_int)(wcslen(m_pUISceneName)), szFileName, MAX_PATH, nullptr, nullptr);

	_char   szPlusText[MAX_PATH] = "_Text";
	_char   szEXT[MAX_PATH] = ".dat";

	_char   szFullPath[MAX_PATH] = "";
	strcat_s(szFullPath, szDir);
	strcat_s(szFullPath, szFileName);
	strcat_s(szFullPath, szPlusText);
	strcat_s(szFullPath, szEXT);

	_tchar  szLastPath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, szFullPath, (_int)strlen(szFullPath), szLastPath, MAX_PATH);

	HANDLE		hFile = CreateFile(szLastPath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Save File - Text", _T("Fail"), MB_OK);
		return S_OK;
	}

	DWORD	dwByte(0);
	_uint iLen = {};
	_float3 fPos = {};

	vector<UI_TextInfo>::iterator it;
	for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
	{
		WriteFile(hFile, &(*it).iTextID, sizeof(_uint), &dwByte, nullptr);

		iLen = (_uint)(*it).strFontName.size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, (*it).strFontName.c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);

		iLen = (_uint)(*it).srtTextContent.size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, (*it).srtTextContent.c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);


		WriteFile(hFile, &(*it).fTextStartPos, sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &(*it).fTextSize, sizeof(_float2), &dwByte, nullptr);
	}


	CloseHandle(hFile);

	MessageBox(g_hWnd, L"텍스트 Save 완료", _T("성공"), MB_OK);
	return S_OK;
}

HRESULT CLevel_GamePlay::LoadData_Text_Scene()
{
	char   szDir[MAX_PATH] = "../Bin/DataFiles/UISave/";
	_char   szFileName[MAX_PATH] = "";

	WideCharToMultiByte(CP_ACP, 0, m_pUISceneName, (_int)(wcslen(m_pUISceneName)), szFileName, MAX_PATH, nullptr, nullptr);

	_char   szPlusText[MAX_PATH] = "_Text";
	_char   szEXT[MAX_PATH] = ".dat";

	_char   szFullPath[MAX_PATH] = "";
	strcat_s(szFullPath, szDir);
	strcat_s(szFullPath, szFileName);
	strcat_s(szFullPath, szPlusText);
	strcat_s(szFullPath, szEXT);

	_tchar  szLastPath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, szFullPath, (_int)strlen(szFullPath), szLastPath, MAX_PATH);

	HANDLE		hFile = CreateFile(szLastPath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, szLastPath, TEXT("Fail"), MB_OK);
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

	return S_OK;
}

HRESULT CLevel_GamePlay::Delete_TextContainer_ALL()
{
	m_TextInfo.clear();

	return S_OK;
}

HRESULT CLevel_GamePlay::Delete_TextContainer_Last()
{
	m_TextInfo.pop_back();
	return S_OK;
}

HRESULT CLevel_GamePlay::Delete_TextContainer_Choice()
{
	vector<UI_TextInfo>::iterator it;
	for (it = m_TextInfo.begin(); it != m_TextInfo.end();)
	{

		if((*it).iTextID == m_CurrentText.iTextID)
		{
			it = m_TextInfo.erase(it);
		}
		else
		{
			it++;
		}
	
	}
	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
	delete[] m_pSaveName;

	map<_wstring, ID3D11ShaderResourceView*>::iterator it;
	for (it = m_ShareSRVs.begin(); it != m_ShareSRVs.end(); ++it)
	{
		Safe_Release((*it).second);
	}	
	for (it = m_MenuSRVs.begin(); it != m_MenuSRVs.end(); ++it)
	{
		Safe_Release((*it).second);
	}	
	for (it = m_LevelUpSRVs.begin(); it != m_LevelUpSRVs.end(); ++it)
	{
		Safe_Release((*it).second);
	}	
	for (it = m_AttributeSRVs.begin(); it != m_AttributeSRVs.end(); ++it)
	{
		Safe_Release((*it).second);
	}for (it = m_PlayerSRVs.begin(); it != m_PlayerSRVs.end(); ++it)
	{
		Safe_Release((*it).second);
	}
	m_ShareSRVs.clear();
	m_MenuSRVs.clear();
	m_LevelUpSRVs.clear();
	m_AttributeSRVs.clear();
	m_PlayerSRVs.clear();

}