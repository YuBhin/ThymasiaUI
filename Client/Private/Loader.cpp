#include "pch.h" 
#include "Loader.h"
#include "Body_Player.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "BackGround.h"
#include "Weapon.h"
#include "Player.h"
#include "LobTrap.h"

#pragma region 보스 
#include "Boss_Magician.h"
#include "Boss_Varg.h"
#pragma endregion 

#pragma region 환경요소 
#include "Sky.h"
#include "Terrain.h"
#pragma endregion 

#pragma region 테스트용 사다리
#include "Ladder.h"
#pragma endregion

#pragma region  UIs
#include "UI_TextBox.h"

#include "UI_SkillIcon.h"
#include "UI_Effect_Frame.h"
#include "UI_Effect_SlotFrame.h"

#include "UIGroup_GameIntro.h"
#include "UIGroup_PlayerMenu.h"
#include "UIGroup_PlayerLevelUP.h"
#include "UIGroup_PlayerTalent.h"
#include "UIGroup_PlayerScreen.h"
#include "UIGroup_Iventory.h"

#include "UI_MouseCursor.h"

#include "UI_NewHint.h"
#include "UI_LockedIcon.h"
#include "UI_LevelImage.h"
#include "UI_KeyBox_Long.h"
#include "UI_Indicator_Selected.h"
#include "UI_HightlightBar.h"
#include "UI_DiamondIcon.h"
#include "UI_RightBackground.h"
#include "UI_LeftBackground.h"
#include "UI_ButtonHighlight.h"
#include "UI_DecorationLine.h"

#include "UI_SceneAboveBackground.h"
#include "UI_SceneBackground.h"
#include "UI_SceneUnderBackground.h"
#include "UI_UnderLine.h"
#include "UI_Arrow.h"
#include "UI_Attribute_Slot_Active.h"
#include "UI_BaseTalentIndicator.h"
#include "UI_ContentBackground.h"
#include "UI_DescBackground.h"
#include "UI_Frame.h"

#include "UI_Arrow_Long.h"
#include "UI_BlackBar.h"
#include "UI_DecorationFrame.h"
#include "UI_ButtonBackground.h"
#include "UI_TitleBackgroundBar.h"


#include "UI_EmptyItemBox.h"
#include "UI_ForgottenFeatherIcon.h"
#include "UI_ItemBackground.h"
#include "UI_SquareFrame.h"
#include "UI_SquareFrame_Hover.h"
#include "UI_MemoryCollectionIcon.h"
#include "UI_Item_Icon.h"

#include "UI_Feather_Icon.h"
#include "UI_HPBar1_Border.h"
#include "UI_HPBar2_BG.h"
#include "UI_HPBar3_MainBar.h"
#include "UI_HPBar5_Track.h"
#include "UI_MemoryShardsIcon.h"
#include "UI_MPBar1_Border.h"
#include "UI_MPBar2_BG.h"
#include "UI_MPBar3_MainBar.h"
#include "UI_PlunderSlotFrame.h"
#include "UI_FixSlotFrame.h"
#include "UI_Potion_DefaultType.h"
#include "UI_DialogBackground.h"

#include "UI_GameLogoImage.h"
#include "UI_LoadingScreen.h"
#include "UI_LoadingIcon.h"
#include "UI_LootNotifyBackground.h"
#include "UI_Talent_Icon.h"
#include "UI_LandingScreen.h"

#include "UI_MapEntryFrame.h"
#include "UI_Map_Save_Image.h"
#include "UI_MediaFrame.h"

#include "UI_Skill_Slot.h"

#include "UI_DialogueTalkBackground.h"
#include "UI_DialogueWindowBackground.h"
#include "UI_NextLineHint.h"

#include "UI_QuestBackground.h"
#include "UI_Bar_Poison.h"
#include "UI_Frame_Poison.h"
#pragma endregion


CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance { CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

_uint APIENTRY Thread_Main(void* pArg)
{
	CLoader*		pLoader = static_cast<CLoader*>(pArg);

 	pLoader->Start_Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{


	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	// m_eNextLevelID가 필요로하는 자원을 로드해주기위한 스레드를 만들어낸다. 
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Start_Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	CoInitializeEx(nullptr, 0);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_Level_GamePlay();
		break;
	case LEVEL_UITEST:
		hr = Loading_For_Level_GamePlay();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

void CLoader::SetUp_WindowText()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Loading_For_Level_Logo()
{

#pragma region Logo 
	lstrcpyW(m_szLoadingText, TEXT("텍스쳐 원형을 생성한다."));
	/* For.Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/LoadingScreen/LoadingScreen_%d.dds"), 8))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("셰이더 원형을 생성한다."));
	//D3D11_INPUT_ELEMENT_DESC	VertexElements[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA , 0 },			
	//};

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex_UI.hlsl"), VTXPOSTEX_UI::Elements, VTXPOSTEX_UI::iNumElements))))
		return E_FAIL;


	lstrcpyW(m_szLoadingText, TEXT("모델 원형을 생성한다."));
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpyW(m_szLoadingText, TEXT("객체 원형을 생성한다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;	

	/* 로딩이 완료되었습ㄴ미다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));
	m_isFinished = true;

	return S_OK;
#pragma endregion 
}

HRESULT CLoader::Loading_For_Level_GamePlay()
{
	/* 게임플레이레벨을 위한 자원을 로드합니다.  */
	lstrcpyW(m_szLoadingText, TEXT("게임플레이레벨을 위한 자원을 로딩 중입니다."));

	// 초기 설정을 위한 트랜스폼 
	_matrix PreTransformMatrix = XMMatrixIdentity();	

#pragma region 지형
	lstrcpyW(m_szLoadingText, TEXT("텍스쳐 원형을 생성한다."));
	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 100, 100, 1, nullptr))))
		return E_FAIL;


	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),	
		CTerrain::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	
#pragma endregion 



#pragma region Sky

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_3.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 




#pragma region 카메라 
	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region 셰이더

	lstrcpyW(m_szLoadingText, TEXT("셰이더 원형을 생성한다."));
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Shader_VtxMeshNoDefferd */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshNoDefferd"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMeshNoDefferd.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))	
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMeshNoDefferd */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMeshNoDefferd"),	
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMeshNoDefferd.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))	
		return E_FAIL;	


	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTexInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTexInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTexInstance.hlsl"), VTX_RECT_INSTANCE::Elements, VTX_RECT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl"), VTX_POINT_INSTANCE::Elements, VTX_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex_UI.hlsl"), VTXPOSTEX_UI::Elements, VTXPOSTEX_UI::iNumElements))))
		return E_FAIL;

#pragma endregion 


#pragma region 카쿠 모델
	//lstrcpyW(m_szLoadingText, TEXT("카쿠 모델을 생성한다."));
	///* For.Prototype_Component_Model_Kaku*/
	//PreTransformMatrix = /*XMMatrixScaling(0.015f, 0.015f, 0.015f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Kaku"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Kaku/2_03_kaku_2.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
	//	return E_FAIL;
	//
	/////* For.Prototype_GameObject_Player */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),
	//	CPlayer::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Body_Player */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Body_Player"),
	//	CBody_Player::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

#pragma endregion 

#pragma region 카쿠 무기 모델 
	//lstrcpyW(m_szLoadingText, TEXT("카쿠 무기 모델을 생성한다."));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Kaku_Weapon"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/KakuWeapon/weapon.fbx", CModel::MODEL_NONANIM))))
	//	return E_FAIL;
	//
	//
	/////* For.Prototype_GameObject_Weapon */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Weapon"),
	//	CWeapon::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;	

#pragma endregion 

#pragma region Navigation 
	lstrcpyW(m_szLoadingText, TEXT("네비게이션 원형을 생성한다."));	
	/* For.Prototype_Component_Navigation */	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),	
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Navigation_File/test49.bin")))))	
		return E_FAIL;	

#pragma endregion 

#pragma region Collider

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext,CCollider::TYPE_AABB))))	
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))	
		return E_FAIL;

#pragma endregion


#pragma region UI 텍스쳐

	lstrcpyW(m_szLoadingText, TEXT("UI 생성 시작."));

	//====================================================================================================================================== 게임 인트로

	/* For.Prototype_Component_Texture_UI_LoadingScreen */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_LoadingScreen"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/LoadingScreen/LoadingScreen_%d.dds"), 8))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LoadingScreen */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LoadingScreen"),
		CUI_LoadingScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_LoadingIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_LoadingIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/LoadingScreen/UI_LoadingIcon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LoadingIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LoadingIcon"),
		CUI_LoadingIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_KeyBox_Long*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_GameLogoImage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/GameIntro/UI_GameLogoImage.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KeyBox_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_GameLogoImage"),
		CUI_GameLogoImage::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================== 이펙트
	/* For.Prototype_Component_Texture_UI_Effect_Frame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Effect_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Effect_Frame_%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Effect_Frame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Effect_Frame"),
		CUI_Effect_Frame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_UI_Effect_SlotFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Effect_SlotFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Effect_SlotFrame_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Effect_SlotFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Effect_SlotFrame"),
		CUI_Effect_SlotFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_UI_Effect_SlotFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SkillIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_SkillIcon_%d.dds"), 22))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Effect_SlotFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SkillIcon"),
		CUI_SkillIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	
	//====================================================================================================================================== 키보드 텍스처
	/* For.Prototype_Component_Texture_UI_KeyBox_Long*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_KeyBox_Long"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_KeyBox_Long.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KeyBox_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_KeyBox_Long"),
		CUI_KeyBox_Long::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//====================================================================================================================================== 텍스트 박스
	

	/* For.Prototype_GameObject_UI_TextBox */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_TextBox"),
		CUI_TextBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	
	//====================================================================================================================================== 공용 꾸밈 텍스처

	/* For.Prototype_Component_Texture_UI_ButtonHighlight*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ButtonHighlight"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_ButtonHighlight_0%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_ButtonHighlight */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ButtonHighlight"),
		CUI_ButtonHighlight::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	//====================================================================================================================================== 공용 꾸밈 텍스처
	

	/* For.Prototype_Component_Texture_UI_DecorationLine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DecorationLine"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_DecorationLine_%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_DecorationLine */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DecorationLine"),
		CUI_DecorationLine::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================== 넛지 텍스처

	/* For.Prototype_Component_Texture_UI_DiamondIcon_01*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DiamondIcon_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_DiamondIcon_01.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DiamondIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DiamondIcon"),
		CUI_DiamondIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Indicator_Selected*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Indicator_Selected"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_Indicator_Selected_%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Indicator_Selected */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Indicator_Selected"),
		CUI_Indicator_Selected::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LockedIcon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_LockedIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_LockedIcon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LockedIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LockedIcon"),
		CUI_LockedIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_NewHint*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_NewHint"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_NewHint.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_NewHint */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_NewHint"),
		CUI_NewHint::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//==================================================================================================================================== 플레이어 메뉴창(의자착석)

		/* For.Prototype_Component_Texture_UI_HighlightBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HighlightBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_HighlightBar_0%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_HighlightBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HighlightBar"),
		CUI_HightlightBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

		/* For.Prototype_Component_Texture_UI_LeftBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_LeftBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_LeftBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LeftBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LeftBackground"),
		CUI_LeftBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

		/* For.Prototype_Component_Texture_UI_RightBackground*/
 	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_RightBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_RightBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_RightBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_RightBackground"),
		CUI_RightBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LevelImage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_LevelImage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_LevelImage_%d.dds"), 4))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelImage */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LevelImage"),
		CUI_LevelImage::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//==================================================================================================================================== 플레이어 특성 창

	/* For.Prototype_Component_Texture_UI_Arrow*/
 	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Arrow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_Arrow.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Arrow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Arrow"),
		CUI_Arrow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Attribute_Slot_Active*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Attribute_Slot_Active"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_Attribute_Slot_Active_%d.dds"), 3))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Attribute_Slot_Active */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Attribute_Slot_Active"),
		CUI_Attribute_Slot_Active::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_BaseTalentIndicator*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_BaseTalentIndicator"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_BaseTalentIndicator.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_BaseTalentIndicator */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_BaseTalentIndicator"),
		CUI_BaseTalentIndicator::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_ContentBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ContentBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_ContentBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_ContentBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ContentBackground"),
		CUI_ContentBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_DescBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DescBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_DescBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DescBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DescBackground"),
		CUI_DescBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Frame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_Frame_%d.dds"), 4))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Frame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Frame"),
		CUI_Frame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SceneAboveBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SceneAboveBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_SceneAboveBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SceneAboveBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SceneAboveBackground"),
		CUI_SceneAboveBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SceneBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SceneBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_SceneBackground_%d.dds"), 3))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SceneBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SceneBackground"),
		CUI_SceneBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SceneUnderBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SceneUnderBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_SceneUnderBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SceneUnderBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SceneUnderBackground"),
		CUI_SceneUnderBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_UnderLine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_UnderLine"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerAttribute/UI_UnderLine.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_UnderLine */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_UnderLine"),
		CUI_UnderLine::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_Talent_Icon*/
 	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Talent_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/TalentIcon/UI_Talent_Icon_%d.dds"), 39))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Talent_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Talent_Icon"),
		CUI_Talent_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//==================================================================================================================================== 플레이어 레벨 업 창

		/* For.Prototype_Component_Texture_UI_Arrow_Long*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Arrow_Long"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerLevelUP/UI_Arrow_Long.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Arrow_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Arrow_Long"),
		CUI_Arrow_Long::Create(m_pDevice, m_pContext))))
		return E_FAIL;

		/* For.Prototype_Component_Texture_UI_BlackBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_BlackBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerLevelUP/UI_BlackBar.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_BlackBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_BlackBar"),
		CUI_BlackBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
		/* For.Prototype_Component_Texture_UI_DecorationFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DecorationFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerLevelUP/UI_DecorationFrame_01.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DecorationFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DecorationFrame"),
		CUI_DecorationFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
		/* For.Prototype_Component_Texture_UI_SkillLeftBackground*/
 	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ButtonBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerLevelUP/UI_ButtonBackground_%d.dds"), 3))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SkillLeftBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ButtonBackground"),
		CUI_ButtonBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
		/* For.Prototype_Component_Texture_UI_TitleBackgroundBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_TitleBackgroundBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerLevelUP/UI_TitleBackgroundBar_0%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_TitleBackgroundBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_TitleBackgroundBar"),
		CUI_TitleBackgroundBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//==================================================================================================================================== 플레이어 기본 화면

	/* For.Prototype_Component_Texture_UI_LandingScreen*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_LandingScreen"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Message/UI_LandingScreen_%d.dds"), 11))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LandingScreen */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LandingScreen"),
		CUI_LandingScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_UI_Feather_Icon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Feather_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_Feather_Icon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Feather_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Feather_Icon"),
		CUI_Feather_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_HPBar1_Border*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HPBar1_Border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_HPBar1_Border.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_HPBar1_Border */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HPBar1_Border"),
		CUI_HPBar1_Border::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	
	/* For.Prototype_Component_Texture_UI_TitleBackgroundBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HPBar2_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_HPBar2_BG.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_TitleBackgroundBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HPBar2_BG"),
		CUI_HPBar2_BG::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	
	/* For.Prototype_Component_Texture_UI_HPBar3_MainBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HPBar3_MainBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_HPBar3_MainBar.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_HPBar3_MainBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HPBar3_MainBar"),
		CUI_HPBar3_MainBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	
	/* For.Prototype_Component_Texture_UI_TitleBackgroundBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MemoryShardsIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_MemoryShardsIcon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_TitleBackgroundBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MemoryShardsIcon"),
		CUI_MemoryShardsIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	
	/* For.Prototype_Component_Texture_UI_MPBar1_Border*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MPBar1_Border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_MPBar1_Border.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MPBar1_Border */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MPBar1_Border"),
		CUI_MPBar1_Border::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	
	/* For.Prototype_Component_Texture_UI_MPBar2_BG*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MPBar2_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_MPBar2_BG.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MPBar2_BG */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MPBar2_BG"),
		CUI_MPBar2_BG::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	
	/* For.Prototype_Component_Texture_UI_MPBar3_MainBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MPBar3_MainBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_MPBar3_MainBar.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MPBar3_MainBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MPBar3_MainBar"),
		CUI_MPBar3_MainBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;	

	/* For.Prototype_Component_Texture_UI_HPBar5_Track*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HPBar5_Track"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_HPBar5_Track.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_HPBar5_Track */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HPBar5_Track"),
		CUI_HPBar5_Track::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_UI_PlunderSlotFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_PlunderSlotFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_PlunderSlotFrame_%d.dds"), 3))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_PlunderSlotFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_PlunderSlotFrame"),
		CUI_PlunderSlotFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	
	/* For.Prototype_GameObject_UI_FixSlotFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_FixSlotFrame"),
		CUI_FixSlotFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	
	/* For.Prototype_Component_Texture_UI_Potion_DefaultType*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Potion_DefaultType"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMainScreen/UI_Potion_DefaultType.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Potion_DefaultType */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Potion_DefaultType"),
		CUI_Potion_DefaultType::Create(m_pDevice, m_pContext))))
		return E_FAIL;	

	
	/* For.Prototype_Component_Texture_UI_LootNotifyBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_LootNotifyBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_LootNotifyBackground_0%d.dds"), 4))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LootNotifyBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LootNotifyBackground"),
		CUI_LootNotifyBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;	


	//====================================================================================================================================== 인벤토리

	/* For.Prototype_Component_Texture_UI_EmptyItemBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_EmptyItemBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_EmptyItemBox.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_EmptyItemBox */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_EmptyItemBox"),
		CUI_EmptyItemBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_ForgottenFeatherIcon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ForgottenFeatherIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_ForgottenFeatherIcon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_ForgottenFeatherIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ForgottenFeatherIcon"),
		CUI_ForgottenFeatherIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_ItemBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ItemBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_ItemBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_ItemBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ItemBackground"),
		CUI_ItemBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SquareFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SquareFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_SquareFrame.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SquareFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SquareFrame"),
		CUI_SquareFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SquareFrame_Hover*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SquareFrame_Hover"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_SquareFrame_Hover.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SquareFrame_Hover */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SquareFrame_Hover"),
		CUI_SquareFrame_Hover::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_MemoryCollectionIcon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MemoryCollectionIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_MemoryCollectionIcon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MemoryCollectionIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MemoryCollectionIcon"),
		CUI_MemoryCollectionIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Item_Icon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Item_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_Item_Icon_%d.dds"), 8))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Item_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Item_Icon"),
		CUI_Item_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_DialogBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DialogBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_DialogBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DialogBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DialogBackground"),
		CUI_DialogBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================== 맵?

	/* For.Prototype_Component_Texture_UI_MapEntryFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MapEntryFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/MapChange/UI_MapEntryFrame_%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MapEntryFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MapEntryFrame"),
		CUI_MapEntryFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Map_Save_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Map_Save_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/MapChange/UI_MapTabImage_%d.dds"), 4))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MapEntryFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Map_Save_Image"),
		CUI_Map_Save_Image::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_MediaFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MediaFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/MapChange/UI_MediaFrame.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MediaFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MediaFrame"),
		CUI_MediaFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//==================================================================================================================================== 스킬


	/* For.Prototype_GameObject_UI_Skill_Slot */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Skill_Slot"),
		CUI_Skill_Slot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//====================================================================================================================================== 아이세미 대화


	/* For.Prototype_Component_Texture_UI_DialogueTalkBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DialogueTalkBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Dialogue/UI_DialogueTalkBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DialogueTalkBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DialogueTalkBackground"),
		CUI_DialogueTalkBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_DialogueWindowBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DialogueWindowBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Dialogue/UI_DialogueWindowBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DialogueWindowBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DialogueWindowBackground"),
		CUI_DialogueWindowBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_NextLineHint*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_NextLineHint"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Dialogue/UI_NextLineHint_%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_NextLineHint */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_NextLineHint"),
		CUI_NextLineHint::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//==================================================================================================================================== Poison 독 게이지


	/* For.Prototype_Component_Texture_UI_QuestBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_QuestBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Poison/UI_QuestBackground.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Bar_Poison*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Bar_Poison"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Poison/UI_Bar_Poison.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Frame_Poison*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Frame_Poison"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Poison/UI_Frame_Poison.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Icon_Poison*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Icon_Poison"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Poison/UI_Icon_Poison.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Bar_Noise*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Bar_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Poison/UI_Bar_Noise.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_BarGlow_Poison*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_BarGlow_Poison"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Poison/UI_BarGlow_Poison.dds"), 1))))
		return E_FAIL;
	

	/* For.Prototype_GameObject_UI_QuestBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_QuestBackground"),
		CUI_QuestBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UI_Bar_Poison */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Bar_Poison"),
		CUI_Bar_Poison::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UI_Frame_Poison */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Frame_Poison"),
		CUI_Frame_Poison::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	
	//====================================================================================================================================


	/* For.Prototype_Component_Texture_MouseCursor*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_MouseCursor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/TexUI_GameCursor.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_VIBuffer_SliceNine */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_SliceNine"),
		CVIBuffer_SliceNine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_GameIntro"),
		CUIGroup_GameIntro::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_TestScene_TextTest */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerMenu"),
		CUIGroup_PlayerMenu::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerLevelUP"),
		CUIGroup_PlayerLevelUP::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerTalent"),
		CUIGroup_PlayerTalent::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerScreen"),
		CUIGroup_PlayerScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UIGroup_Iventory */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_Iventory"),
		CUIGroup_Iventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	///* For.Prototype_GameObject_UI_Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Component"),
	//	CUI_LeftBackground::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	/* For.Prototype_GameObject_UI_Component */
	/*if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Slot_Attribute"),
		CUI_AttributeButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;*/
	/* For.Prototype_GameObject_UI_MouseCursor */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MouseCursor"),
		CUI_MouseCursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion


#pragma region 티메시아 캐릭터 
	lstrcpyW(m_szLoadingText, TEXT("티메시아 모델을 생성한다."));	
	/* For.Prototype_Component_Model_Kaku*/
	PreTransformMatrix = /*XMMatrixScaling(0.015f, 0.015f, 0.015f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));		
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corner"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Thymesia/realtest.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))	
		return E_FAIL;		

	///* For.Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Body_Player"),	
		CBody_Player::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	
	
	///* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),	
		CPlayer::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LobTrap"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Thymesia/LobTrap.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_LobTrap"),
		CLobTrap::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 


#pragma region 테스트용 사다리
	lstrcpyW(m_szLoadingText, TEXT("사다리 모델을 생성한다."));
	/* For.Prototype_Component_Model_Ladder*/
	PreTransformMatrix = /*XMMatrixScaling(0.015f, 0.015f, 0.015f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ladder"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Ladder/Ladder.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Ladder"),
		CLadder::Create(m_pDevice, m_pContext))))
		return E_FAIL;



#pragma endregion 
#pragma region 티메시아 보스

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Magician"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Magician/Boss_Magician.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Magician"),
		CBoss_Magician::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Varg"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Varg/Boss_Varg.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Varg"),
		CBoss_Varg::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 
	/* 로딩이 완료되었습ㄴ미다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));	
	m_isFinished = true;
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
