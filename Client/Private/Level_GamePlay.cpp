#include "pch.h" 
#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"	

#include "UI_LeftBackground.h"
#include "UI_AttributeButton.h"
#include "UI_Scene.h"
#include "UI_Button.h"
#include "UI_Button_Player.h"
#include "UI_Image.h"
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

	if (FAILED(Ready_Layer_UI_Scene(TEXT("Layer_UIScene"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Ladder(TEXT("Layer_Ladder"))))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;


	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{

	if (m_pGameInstance->isMouseEnter(DIM_RB))
	{
		if (m_bUIorText)
			Create_UI_Scene_Object();
		else
			Create_Text();

	}
	//if (m_pGameInstance->isKeyEnter(DIK_E))
	//{
	//	m_iOpenSceneCount++;
	//	if (1 == m_iOpenSceneCount) // 레벨업
	//	{
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), true);
	//		//pUIScene->UIScene_UIObject_Render_OnOff(true);
	//	}
	//	if (2 == m_iOpenSceneCount) // 특성
	//	{
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), false);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_ATTRIBUTE, L"UIScene_PlayerAttribute")), true);
	//	}
	//	if (3 == m_iOpenSceneCount) // 초기화
	//	{
	//		m_iOpenSceneCount = 0;

	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), false);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_ATTRIBUTE, L"UIScene_PlayerAttribute")), false);
	//	}
	//}

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
		m_pGameInstance->Clear_Choice(m_iUIType, m_iSceneIndex, m_szUISceneName, m_pCurrentUIObj);

	}
}

HRESULT CLevel_GamePlay::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
#endif

	ShowGUI();
	ImGui::Render();

	m_pGameInstance->Render_Font(m_szFontName, m_szTextContent, TextTest);

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

HRESULT CLevel_GamePlay::Ready_Layer_UI_Scene(const _tchar* pLayerTag)
{
	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LevelUP"), LEVEL_GAMEPLAY, pLayerTag)))
	//	return E_FAIL;
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

		// 방향키가 영향을 주지 않게
	ImGui::GetIO().NavActive = false;
	// 마우스가 영향을 주도록
	ImGui::GetIO().WantCaptureMouse = true;

	ImGui::Begin(u8"UI Tool", NULL, ImGuiWindowFlags_MenuBar);//창 이름

	SetUIFont();
	SetUIScene();
	//메뉴바
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			//메뉴
			if (ImGui::MenuItem("UISave"))
			{
				if (m_bUIorText)
					SaveData_UI_Scene(m_iSceneIndex, m_szUISceneName);
				else
					MessageBox(g_hWnd, L"UI 선택 후 저장하기", _T("Fail"), MB_OK);


			}
			ImGui::Separator();
			if (ImGui::MenuItem("UILoad"))
			{
				if (m_bUIorText)
					LoadData_UI_Scene(m_iSceneIndex, m_szUISceneName);
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
			if (ImGui::MenuItem("LastObj_Delete"))
			{
				m_pGameInstance->Clear_Last(m_iUIType, m_iSceneIndex, m_szUISceneName);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Text_Delete_ALL"))
			{
				Delete_TextContainer_ALL();
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

	ImGui::NewLine();

	static _int iradioSelect0 = 0;
	static _int iradioSelect1 = 0;

	ImGui::Text(u8"UI 선택 상태");
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


	ImGui::Text(u8"UIScene");

	static _int iradioSelect2 = 0;


	//초기값 설정
	if (iradioSelect2 == 0)
	{
		m_iUIScenIndex = 0;
		m_szUISceneName = { L"UIScene_PlayerMenu" };
		m_iSceneIndex = UISCENE_MENU;
	}
	if (ImGui::RadioButton("PlayerMenu", iradioSelect2 == 0)) {
		iradioSelect2 = 0;
		m_iUIScenIndex = 0;
		m_szUISceneName = { L"UIScene_PlayerMenu" };
		m_iSceneIndex = UISCENE_MENU;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("PlayerLevelUP", iradioSelect2 == 1)) {
		iradioSelect2 = 1;
		m_iUIScenIndex = 1;
		m_szUISceneName = { L"UIScene_PlayerLevelUP" };
		m_iSceneIndex = UISCENE_LEVELUP;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("PlayerAttribute", iradioSelect2 == 2)) {
		iradioSelect2 = 2;
		m_iUIScenIndex = 2;
		m_szUISceneName = { L"UIScene_PlayerAttribute" };
		m_iSceneIndex = UISCENE_ATTRIBUTE;
	}

	SetUIObj_Type();
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
	if (ImGui::RadioButton("UI_TEXTPLAYER", iradioSelect1 == 3)) {
		iradioSelect1 = 3;
		m_iUIType = 3;
	}

	ImGui::NewLine();
	SetUISetting();
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

	CUI_Scene* pUIScene = m_pGameInstance->Find_UIScene(m_iSceneIndex, m_szUISceneName);
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
					fGetsize = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Get_Scale_UIObj();
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
					fGetsize = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Get_Scale_UIObj();
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
					fGetsize = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Get_Scale_UIObj();
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
				for (auto& UIObj : pUIScene->Find_UI_Text_PlayerInfo())
				{
					fGetPos = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
					fGetsize = dynamic_cast<CTransform*>(UIObj->Find_Component(TEXT("Com_Transform")))->Get_Scale_UIObj();
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
	ImGui::NewLine();

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
	ImGui::NewLine();

	ImGui::Text(u8"UI 회전");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputFloat3("UIRotation", &fRotation.x, "%.2f"))
	{
		fRotation.x = XMConvertToRadians(fRotation.x);
		fRotation.y = XMConvertToRadians(fRotation.y);
		fRotation.z = XMConvertToRadians(fRotation.z);
		if (!m_bGetUIObjCheck)// 현재 객체가 명확하게 선택 되어 있는 경우에만 작동
		{
			if (nullptr != m_pCurrentUIObj)
			{
				dynamic_cast<CTransform*>(m_pCurrentUIObj->Find_Component(TEXT("Com_Transform")))->Set_UIObj_Rotation(fRotation.z);

			}
		}
		m_fRotation = fRotation;
	}
	ImGui::NewLine();

	static _int iShaderPassNum = { 12 };
	ImGui::Text(u8"쉐이더 패스 번호");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputInt("패스번호", &iShaderPassNum))
	{
		m_pCurrentUIObj->Set_UI_ShaderPassNum(iShaderPassNum);
	}

	ImGui::NewLine();
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

	map<_wstring, ID3D11ShaderResourceView*>::iterator it;
	for (it = m_SRVs.begin(); it != m_SRVs.end(); ++it)
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
			m_szUIName = (*it).first.c_str();
			iSelectButton = iTemp;
			//m_iNumber = iTemp;
		}

		iCount++;
		if (iCount < 8)
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

	ImGui::Text(u8"FontInfo");
	ImGui::Separator();

	//ImGui::Text("Score: %d", score);              // 정수 출력
	//ImGui::Text("Time Elapsed: %.2f seconds", timeElapsed); // 실수 출력 (소수점 2자리)
	//ImGui::Text("Message: %s", text.c_str()); // 문자열 출력

	ImGui::BulletText("TextID: %d", m_CurrentText.iTextID);
	ImGui::BulletText(u8"사용 폰트: %s", m_CurrentText.strFontName.c_str());
	ImGui::BulletText(u8"텍스트 내용: %s", m_CurrentText.srtTextContent.c_str());
	ImGui::BulletText(u8"가로 세로: %.2f / %.2f", m_CurrentText.fTextSize.x, m_CurrentText.fTextSize.y);
	ImGui::BulletText("X Y : %.2f / %.2f", m_CurrentText.fTextStartPos.x, m_CurrentText.fTextStartPos.y);
	ImGui::Separator();


	ImGui::Text(u8"Font Text 입력");
	static _char szInputText[MAX_PATH] = "";
	//	_tchar  szLastPath[MAX_PATH] = {};
	if (ImGui::InputText(u8"텍스트 내용 작성", szInputText, IM_ARRAYSIZE(szInputText)))
	{
		_int iResize = MultiByteToWideChar(CP_UTF8, 0, szInputText, -1, NULL, 0);
		m_szSaveName = new _tchar[iResize];

		MultiByteToWideChar(CP_UTF8, 0, szInputText, -1, m_szSaveName, iResize);

		m_szTextContent = m_szSaveName;

		// 디버깅용 출력
	}
	else
		m_bCurrentText = false;
	ImGui::Text(u8"작성된: %s", szInputText);


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

HRESULT CLevel_GamePlay::Create_UI_Scene_Object()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	CUIObject::UIOBJECT_DESC Desc{};

	if (nullptr == m_szUIName)
	{
		MessageBox(g_hWnd, L"생성할 UI 선택 필요", _T("Fail"), MB_OK);
		return E_FAIL;

	}

	Desc.fSizeX = m_fSize.x;
	Desc.fSizeY = m_fSize.y;
	Desc.fX = (_float)ptMouse.x;
	Desc.fY = (_float)ptMouse.y;
	Desc.fZ = m_fPos.z;
	Desc.szProtoName = m_szUIName;
	Desc.iShaderPassNum = m_iShaderPassNum;
	Desc.fRotation = m_fRotation;
	m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY, m_szUIName, m_iSceneIndex, m_szUISceneName, m_iUIType, &Desc);



	return S_OK;
}

HRESULT CLevel_GamePlay::Save_UI_IMGUI_Textrue()
{
	Save_UI_Textrue(TEXT("Prototype_GameObject_UI_Component"), TEXT("../Bin/Resources/Textures/ThymesiaUI/UI_0.dds"), 1);
	Save_UI_Textrue(TEXT("Prototype_GameObject_UI_Slot_Attribute"), TEXT("../Bin/Resources/Textures/ThymesiaUI/Slot_Attribute_0.dds"), 1);




	return S_OK;
}

HRESULT CLevel_GamePlay::Save_UI_Textrue(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures)
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

		m_SRVs.emplace(_pObjTag, pSRV);
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::SaveData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName) // 파라미터 => Load 시 각 씬 클래스 에서 집어 넣도록 = 여기서는 imgui를 통해서
{
	const _tchar* szUISceneName = L"../Bin/DataFiles/UISave/Defalut.dat";
	if (0 == wcscmp(szSceneName, L"UIScene_PlayerMenu"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerMenu.dat";
	if (0 == wcscmp(szSceneName, L"UIScene_PlayerLevelUP"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerLevelUP.dat";
	if (0 == wcscmp(szSceneName, L"UIScene_PlayerAttribute"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerAttribute.dat";

	HANDLE		hFile = CreateFile(szUISceneName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Save File - UIScene", _T("Fail"), MB_OK);
		return S_OK;
	}

	DWORD	dwByte(0);
	_uint iUIType = {};
	_uint iLen = {};
	_uint iShaderNum = {};
	_float3 fPos = {};

	for (auto& Button : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_Button())
	{
		iUIType = UI_BUTTON;

		WriteFile(hFile, &dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Get_Scale_UIObj(), sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Get_Rotation_UIObj(), sizeof(_float3), &dwByte, nullptr);

		iLen = (_uint)Button->Get_UI_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, Button->Get_UI_Name().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);

		WriteFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		iShaderNum = Button->Get_UI_ShaderPassNum();
		WriteFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);
	}

	for (auto& Button_Player : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_Button_Player())
	{
		iUIType = UI_BUTTONPLAYER;

		WriteFile(hFile, &dynamic_cast<CTransform*>(Button_Player->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &dynamic_cast<CTransform*>(Button_Player->Find_Component(TEXT("Com_Transform")))->Get_Scale_UIObj(), sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &dynamic_cast<CTransform*>(Button_Player->Find_Component(TEXT("Com_Transform")))->Get_Rotation_UIObj(), sizeof(_float3), &dwByte, nullptr);

		iLen = (_uint)Button_Player->Get_UI_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, Button_Player->Get_UI_Name().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);

		WriteFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		iShaderNum = Button_Player->Get_UI_ShaderPassNum();
		WriteFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);
	}

	for (auto& Image : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_Image())
	{
		iUIType = UI_IMAGE;
		WriteFile(hFile, &dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_Scale_UIObj(), sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_Rotation_UIObj(), sizeof(_float3), &dwByte, nullptr);

		iLen = (_uint)Image->Get_UI_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, Image->Get_UI_Name().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);

		WriteFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		iShaderNum = Image->Get_UI_ShaderPassNum();
		WriteFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);
	}

	for (auto& Text_PlayerInfo : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_Text_PlayerInfo())
	{
		iUIType = UI_TEXTPLAYER;

		WriteFile(hFile, &dynamic_cast<CTransform*>(Text_PlayerInfo->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &dynamic_cast<CTransform*>(Text_PlayerInfo->Find_Component(TEXT("Com_Transform")))->Get_Scale_UIObj(), sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &dynamic_cast<CTransform*>(Text_PlayerInfo->Find_Component(TEXT("Com_Transform")))->Get_Rotation_UIObj(), sizeof(_float3), &dwByte, nullptr);

		iLen = (_uint)Text_PlayerInfo->Get_UI_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, Text_PlayerInfo->Get_UI_Name().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);

		WriteFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		iShaderNum = Text_PlayerInfo->Get_UI_ShaderPassNum();
		WriteFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Save 완료", _T("성공"), MB_OK);
	return S_OK;
}

HRESULT CLevel_GamePlay::LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName)
{
	m_pGameInstance->Clear_ALL();


	const _tchar* szUISceneName = L"../Bin/DataFiles/UISave/Defalut.dat";
	if (0 == wcscmp(szSceneName, L"UIScene_PlayerMenu"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerMenu.dat";
	if (0 == wcscmp(szSceneName, L"UIScene_PlayerLevelUP"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerLevelUP.dat";
	if (0 == wcscmp(szSceneName, L"UIScene_PlayerAttribute"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerAttribute.dat";

	HANDLE		hFile = CreateFile(szUISceneName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Load File - UIScene", _T("Fail"), MB_OK);
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

		if (0 == dwByte)
		{
			break;
		}

		Desc.fX = fPos.x;
		Desc.fY = fPos.y;
		Desc.fZ = fPos.z;
		Desc.fSizeX = fSize.x;
		Desc.fSizeY = fSize.y;
		Desc.szProtoName = szSaveName;
		Desc.iShaderPassNum = iShaderNum;
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

	_float2 TextSize = m_pGameInstance->Get_TextSize(m_szFontName, m_szTextContent);//텍스트 가로 세로 길이
	Info.fTextSize = TextSize;

	// 소환하는 좌표와 그려지는 텍스트의 중점을 맞추는 계산
	TextSize.x = ptMouse.x - TextSize.x / 2;
	TextSize.y = ptMouse.y - TextSize.y / 2;
	Info.fTextStartPos = TextSize;

	Info.strFontName = m_szFontName;
	Info.srtTextContent = m_szTextContent;
	Info.iTextID = m_iTextID;

	m_TextInfo.push_back(Info);

	m_iTextID++;

	return S_OK;
}

HRESULT CLevel_GamePlay::SaveData_Text_Scene()
{
	const _tchar* szUISceneName = L"../Bin/DataFiles/UISave/DefalutText.dat";
	if (0 == wcscmp(m_szUISceneName, L"UIScene_PlayerMenu"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerMenuText.dat";
	if (0 == wcscmp(m_szUISceneName, L"UIScene_PlayerLevelUP"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerLevelUPText.dat";
	if (0 == wcscmp(m_szUISceneName, L"UIScene_PlayerAttribute"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerAttributeText.dat";

	HANDLE		hFile = CreateFile(szUISceneName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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
	Delete_TextContainer_ALL();

	const _tchar* szUISceneName = L"../Bin/DataFiles/UISave/DefalutText.dat";
	if (0 == wcscmp(m_szUISceneName, L"UIScene_PlayerMenu"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerMenuText.dat";
	if (0 == wcscmp(m_szUISceneName, L"UIScene_PlayerLevelUP"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerLevelUPText.dat";
	if (0 == wcscmp(m_szUISceneName, L"UIScene_PlayerAttribute"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerAttributeText.dat";

	HANDLE		hFile = CreateFile(szUISceneName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

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

	MessageBox(g_hWnd, L"Text Load 완료", _T("성공"), MB_OK);
	return S_OK;
}

HRESULT CLevel_GamePlay::Delete_TextContainer_ALL()
{
	m_TextInfo.clear();

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

	map<_wstring, ID3D11ShaderResourceView*>::iterator it;
	for (it = m_SRVs.begin(); it != m_SRVs.end(); ++it)
	{
		Safe_Release((*it).second);
	}
	m_SRVs.clear();

	delete[] m_szSaveName;

}
