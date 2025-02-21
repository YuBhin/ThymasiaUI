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

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{
	
}

HRESULT CLevel_GamePlay::Initialize()
{
	Save_UI_IMGUI_Textrue();

	/*if (FAILED(Ready_Lights()))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Structure(TEXT("Layer_Structure"))))
	//	return E_FAIL;	


	// 테스트용 사다리 레이어 준비 함수 호출
	if (FAILED(Ready_Layer_Ladder(TEXT("Layer_Ladder"))))
		return E_FAIL;



	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))	
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_Scene(TEXT("Layer_UIScene"))))
		return E_FAIL;
	
	if (FAILED(Ready_Lights()))	
		return E_FAIL;	

	//m_pGameInstance->CheckGroup(GROUP_TYPE::PLAYER,        GROUP_TYPE::MONSTER);		
	//m_pGameInstance->CheckGroup(GROUP_TYPE::PLAYER_WEAPON, GROUP_TYPE::MONSTER);		
	//m_pGameInstance->CheckGroup(GROUP_TYPE::PLAYER,		   GROUP_TYPE::OBJECT);	
	
	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	if (m_pGameInstance->isMouseEnter(DIM_RB))
	{
		Create_UI_Scene_Object();
	}
	if (m_pGameInstance->isKeyEnter(DIK_E))
	{
		m_iOpenSceneCount++;
		if (1 == m_iOpenSceneCount) // 레벨업
		{
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")),true);
			//pUIScene->UIScene_UIObject_Render_OnOff(true);
		}
		if (2 == m_iOpenSceneCount) // 특성
		{
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_ATTRIBUTE, L"UIScene_PlayerAttribute")), true);
		}
		if (3 == m_iOpenSceneCount) // 초기화
		{
			m_iOpenSceneCount = 0;

			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_ATTRIBUTE, L"UIScene_PlayerAttribute")), false);
		}
	}
}

HRESULT CLevel_GamePlay::Render() 
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
#endif

	ShowGUI();
	ImGui::Render();

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
	LightDesc.vDiffuse   = _float4(0.9f, 0.9f, 0.9f, 1.f);
	LightDesc.vAmbient   = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vSpecular  = _float4(0.3f, 0.3f, 0.3f, 1.f);


	/*LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;	
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);	
	LightDesc.vDiffuse   = _float4(0.9f, 0.9f, 0.9f, 1.f);	
	LightDesc.vAmbient   = _float4(0.5f, 0.5f, 0.5f, 1.f);	
	LightDesc.vSpecular  = _float4(0.3f, 0.3f, 0.3f, 1.f);	*/

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;*/

	//for (size_t i = 0; i < 3; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ForkLift"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
	//		return E_FAIL;
	//}

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Structure(const _tchar* pLayerTag)
{
	/* 여기서 맵 파일 하나하나 다 읽어와야함 */

	_ulong dwByte = {}; 
	//HANDLE hFile = CreateFile(TEXT("../Map_File/real76.bin"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	HANDLE hFile = CreateFile(TEXT("../Map_File/real143.bin"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	while (true)
	{
		_tchar Layer_Name[MAX_PATH] = {}; //레이어 이름										
		ReadFile(hFile, Layer_Name, MAX_PATH, &dwByte, nullptr);

		if (dwByte == 0)
			break;
		/* 이름 작업 */
		_char   Prototype_Name[MAX_PATH] = {};

		ReadFile(hFile, Prototype_Name, MAX_PATH, &dwByte, nullptr);


		_float4x4 WorldMatrix = {};
		ReadFile(hFile, &WorldMatrix, sizeof(_float4x4), &dwByte, nullptr);
		//int a = 4;

		_tchar Translate_wchar[MAX_PATH] = {};
		MultiByteToWideChar(CP_ACP, 0, Prototype_Name, static_cast<_int>(strlen(Prototype_Name)), Translate_wchar, MAX_PATH);

		/* 이제 TRANSFORM만 건들면 될듯함.*/
		//int b = 4;
		if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, Translate_wchar, LEVEL_GAMEPLAY, Layer_Name)))
			return E_FAIL;

		//CTransform* pTrasnform = dynamic_cast<CTransform*>(
		//* Find GaemObject 만들어야 할듯
		// 구분할 수 있는 방법을 생각해봐야할듯.
		map<const _wstring, class CLayer*>* Level_Layers = m_pGameInstance->Get_Layers();		

		auto& Level_GamePlay = Level_Layers[3];

		for (auto& Layers : Level_GamePlay)
		{
			//auto& iter = find(Level_GamePlay.begin(), Level_GamePlay.end(), Layer_Name);	
			auto iter = Level_GamePlay.find(Layer_Name);

			if (iter == Level_GamePlay.end())
				return E_FAIL;

			else
			{
				CTransform* pTranform = dynamic_cast<CTransform*>(
					iter->second->Get_GameObject_List().back()->Find_Component(TEXT("Com_Transform")));	

				pTranform->Set_WorldMatrix(WorldMatrix);	
			}
		}
	}
	CloseHandle(hFile);
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))	
		return E_FAIL;	

	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_LobTrap"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
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

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Magician"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Varg"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar* pLayerTag)
{

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_Scene(const _tchar* pLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LevelUP"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Attribute"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;*/

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_Ladder(const _tchar* pLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Ladder"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;*/

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

HRESULT CLevel_GamePlay::On_Mouse_UI()
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
			dynamic_cast<CAttribute_Button*>(pUIObj)->Set_TexNumber(0);
		}
		else
		{
			dynamic_cast<CAttribute_Button*>(pUIObj)->Set_TexNumber(2);

		}
	}*/

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


	SetUIScene();
	//메뉴바
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			//메뉴
			if (ImGui::MenuItem("Save"))
			{
				SaveData_UI_Scene(m_iSceneIndex, m_szUISceneName);

			}
			ImGui::Separator();

			if (ImGui::MenuItem("Open"))
			{
				LoadData_UI_Scene(m_iSceneIndex, m_szUISceneName);

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Scene_Delete"))
			{
				m_pGameInstance->Clear_ALL();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}


	ImGui::End();
}

void CLevel_GamePlay::SetUIScene()
{

	if (!ImGui::CollapsingHeader("UIScene"))
		return; // gui 숨겼다 보여주는 bool 값


	if (ImGui::RadioButton("PlayerMenu", m_iUIScenIndex == 0)) {
		m_iUIScenIndex = 0; // 선택된 값 업데이트
		m_szUISceneName = { L"UIScene_PlayerMenu" };
		m_iSceneIndex = UISCENE_MENU;
	}
	if (ImGui::RadioButton("PlayerLevelUP", m_iUIScenIndex == 1)) {
		m_iUIScenIndex = 1;
		m_szUISceneName = { L"UIScene_PlayerLevelUP" };
		m_iSceneIndex = UISCENE_LEVELUP;
	}
	if (ImGui::RadioButton("PlayerAttribute", m_iUIScenIndex == 2)) {
		m_iUIScenIndex = 2;
		m_szUISceneName = { L"UIScene_PlayerAttribute" };
		m_iSceneIndex = UISCENE_ATTRIBUTE;
	}

	ImGui::NewLine();
	SetUISetting();
}

void CLevel_GamePlay::SetUISetting()
{
	if (!ImGui::CollapsingHeader("UI"))
		return; // gui 숨겼다 보여주는 bool 값


	static _float3 fPos = {};

	ImGui::Text(u8"설치 좌표");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputFloat3("UIPos", &fPos.x, "%.2f"))
	{
		m_fPos = fPos;


	}
	ImGui::NewLine();

	static _float2 fSize = {};
	ImGui::Text(u8"UI 크기");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputFloat2("UISize", &fSize.x, "%.2f"))
	{
		m_fSize = fSize;

	}
	ImGui::NewLine();


	switch (m_iUIType)
	{
	case 0:
		m_szTypeName = "UI_BUTTON";
		break;
	case 1:
		m_szTypeName = "UI_BUTTONPLAYER";
		break;
	case 2:
		m_szTypeName = "UI_IMAGE";
		break;
	case 3:
		m_szTypeName = "UI_TEXTPLAYER";
		break;
	default:
		m_szTypeName = "NONE";
		break;
	}


	static _int iUIType = {};
	ImGui::Text(u8"UI 타입");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputInt(m_szTypeName, &iUIType))
	{
		m_iUIType = iUIType;

	}
	static _int iShaderPassNum = {};
	ImGui::Text(u8"쉐이더 패스 번호");
	ImGui::SameLine(100.f, 0.f);
	if (ImGui::InputInt("패스번호", &iShaderPassNum))
	{
		m_iShaderPassNum = iShaderPassNum;

	}


	ImGui::NewLine();
	SetUIImage();
}
void CLevel_GamePlay::SetUIImage()
{
	bool ret = false;
	if (!ImGui::CollapsingHeader("UIImage"))
	{
		return;
	}
	// 텍스쳐 선택
	ImGui::Text(u8"뭐든 선택");
	int iCount(0);
	_int iTemp(0);

	map<_wstring, ID3D11ShaderResourceView*>::iterator it;
	for (it = m_SRVs.begin(); it != m_SRVs.end(); ++it)
	{
		_int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first.c_str(), -1, NULL, 0, NULL, NULL);
		_char*  pStr = new _char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first.c_str(), -1, pStr, iStrSize, NULL, NULL);

		ret = ImGui::ImageButton(pStr,(ImTextureID)(*it).second, ImVec2(50.f, 50.f));
		iTemp++;
		iCount++;
		if (iCount < 8)
		{
			ImGui::SameLine();
		}
		else
		{
			iCount = 0;
		}
		if (ret)
		{
			m_szUIName = (*it).first.c_str();
			//m_iNumber = iTemp;
		}
		delete[] pStr;
		pStr = nullptr;
	}
	ImGui::NewLine();
}

HRESULT CLevel_GamePlay::Create_UI_Scene_Object()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);


	CUIObject::UIOBJECT_DESC Desc{};
	Desc.fSizeX = m_fSize.x;
	Desc.fSizeY = m_fSize.y;
	Desc.fX = (_float)ptMouse.x;
	Desc.fY = (_float)ptMouse.y;
	Desc.fZ = m_fPos.z;
	Desc.szProtoName = m_szUIName;
	Desc.iShaderPassNum = m_iShaderPassNum;
	m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY, m_szUIName, m_iSceneIndex, m_szUISceneName, m_iUIType, &Desc);



	return S_OK;
}

HRESULT CLevel_GamePlay::Save_UI_IMGUI_Textrue()
{
	Save_UI_Textrue(TEXT("Prototype_GameObject_UI_Component"),TEXT("../Bin/Resources/Textures/ThymesiaUI/UI_0.dds"), 1);
	Save_UI_Textrue(TEXT("Prototype_GameObject_UI_Slot_Attribute"),TEXT("../Bin/Resources/Textures/ThymesiaUI/Slot_Attribute_0.dds"), 1);

	
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

		m_SRVs.emplace(_pObjTag,pSRV);
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::SaveData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName) // 파라미터 => Load 시 각 씬 클래스 에서 집어 넣도록 = 여기서는 imgui를 통해서
{
	const _tchar* szUISceneName = L"../Bin/DataFiles/UISave/Defalut";
	if(0 == wcscmp(szSceneName, L"UIScene_PlayerMenu"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerMenu.dat";
	if(0 == wcscmp(szSceneName, L"UIScene_PlayerLevelUP"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerLevelUP.dat";
	if(0 == wcscmp(szSceneName, L"UIScene_PlayerAttribute"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerAttribute.dat";

	HANDLE		hFile = CreateFile(szUISceneName,GENERIC_WRITE,NULL,	NULL,CREATE_ALWAYS,	FILE_ATTRIBUTE_NORMAL,NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Save File", _T("Fail"), MB_OK);
		return S_OK;
	}

	DWORD	dwByte(0);
	_uint iUIType = {};
	_uint iLen = {};
	_uint iShaderNum = {};

	for (auto& Button : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_Button())
	{
		iUIType = CUI_Scene::UI_BUTTON;

		WriteFile(hFile, &Button->Get_UI_Position(), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &Button->Get_UI_Size(), sizeof(_float2), &dwByte, nullptr);

		iLen = (_uint)Button->Get_UI_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, Button->Get_UI_Name().c_str(), sizeof( _tchar) * iLen, &dwByte, nullptr);

		WriteFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		iShaderNum = Button->Get_UI_ShaderPassNum();
		WriteFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);

	}

	for (auto& Button_Player : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_Button_Player())
	{
		iUIType = CUI_Scene::UI_BUTTONPLAYER;
		WriteFile(hFile, &Button_Player->Get_UI_Position(), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &Button_Player->Get_UI_Size(), sizeof(_float2), &dwByte, nullptr);

		iLen = (_uint)Button_Player->Get_UI_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, Button_Player->Get_UI_Name().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);

		WriteFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		iShaderNum = Button_Player->Get_UI_ShaderPassNum();
		WriteFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);
	}

	for (auto& Image : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_Image())
	{
		iUIType = CUI_Scene::UI_IMAGE;
		WriteFile(hFile, &Image->Get_UI_Position(), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &Image->Get_UI_Size(), sizeof(_float2), &dwByte, nullptr);

		iLen = (_uint)Image->Get_UI_Name().size();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, Image->Get_UI_Name().c_str(), sizeof(_tchar) * iLen, &dwByte, nullptr);

		WriteFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		iShaderNum = Image->Get_UI_ShaderPassNum();
		WriteFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);
	}

	for (auto& Text_PlayerInfo : m_pGameInstance->Find_UIScene(iSceneIndex, szSceneName)->Find_UI_Text_PlayerInfo())
	{
		iUIType = CUI_Scene::UI_TEXTPLAYER;
		WriteFile(hFile, &Text_PlayerInfo->Get_UI_Position(), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &m_iUIScenIndex, sizeof(_float2), &dwByte, nullptr);

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


	const _tchar* szUISceneName = L"../Bin/DataFiles/UISave/Defalut";
	if (0 == wcscmp(szSceneName, L"UIScene_PlayerMenu"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerMenu.dat";
	if (0 == wcscmp(szSceneName, L"UIScene_PlayerLevelUP"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerLevelUP.dat";
	if (0 == wcscmp(szSceneName, L"UIScene_PlayerAttribute"))
		szUISceneName = L"../Bin/DataFiles/UISave/PlayerAttribute.dat";
	
	HANDLE		hFile = CreateFile(szUISceneName,GENERIC_READ,NULL,	NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

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

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

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
}
