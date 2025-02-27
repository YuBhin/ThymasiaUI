#include "pch.h" 
#include "Loader.h"
#include "Body_Player.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "BackGround.h"
#include "Weapon.h"
#include "Player.h"
#include "LobTrap.h"

#pragma region ���� 
#include "Boss_Magician.h"
#include "Boss_Varg.h"
#pragma endregion 

#pragma region ȯ���� 
#include "Sky.h"
#include "Terrain.h"
#pragma endregion 

#pragma region �׽�Ʈ�� ��ٸ�
#include "Ladder.h"
#pragma endregion

#pragma region  UIs
#include "UI_AttributeButton.h"
#include "UI_LevelUP.h"
#include "UI_PlayerAttribute.h"
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

	// m_eNextLevelID�� �ʿ���ϴ� �ڿ��� �ε����ֱ����� �����带 ������. 
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
	lstrcpyW(m_szLoadingText, TEXT("�ؽ��� ������ �����Ѵ�."));
	/* For.Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/LoadingScreen/LoadingScreen_%d.dds"), 8))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("���̴� ������ �����Ѵ�."));
	//D3D11_INPUT_ELEMENT_DESC	VertexElements[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA , 0 },			
	//};

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;


	lstrcpyW(m_szLoadingText, TEXT("�� ������ �����Ѵ�."));
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("��ü ������ �����Ѵ�."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;	

	/* �ε��� �Ϸ�Ǿ������̴� */
	lstrcpyW(m_szLoadingText, TEXT("�ε���."));
	m_isFinished = true;

	return S_OK;
#pragma endregion 
}

HRESULT CLoader::Loading_For_Level_GamePlay()
{
	/* �����÷��̷����� ���� �ڿ��� �ε��մϴ�.  */
	lstrcpyW(m_szLoadingText, TEXT("�����÷��̷����� ���� �ڿ��� �ε� ���Դϴ�."));

	// �ʱ� ������ ���� Ʈ������ 
	_matrix PreTransformMatrix = XMMatrixIdentity();	

#pragma region ����
	lstrcpyW(m_szLoadingText, TEXT("�ؽ��� ������ �����Ѵ�."));
	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("�ͷ��� ������Ʈ ����"));
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




#pragma region ī�޶� 
	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region ���̴�

	lstrcpyW(m_szLoadingText, TEXT("���̴� ������ �����Ѵ�."));
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

#pragma endregion 


#pragma region ī�� ��
	//lstrcpyW(m_szLoadingText, TEXT("ī�� ���� �����Ѵ�."));
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

#pragma region ī�� ���� �� 
	//lstrcpyW(m_szLoadingText, TEXT("ī�� ���� ���� �����Ѵ�."));
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
	lstrcpyW(m_szLoadingText, TEXT("�׺���̼� ������ �����Ѵ�."));	
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


#pragma region UI �ؽ���

	lstrcpyW(m_szLoadingText, TEXT("UI ���� ����."));

	//====================================================================================================================================== Ű���� �ؽ�ó
	/* For.Prototype_Component_Texture_UI_KeyBox_Long*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_KeyBox_Long"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_KeyBox_Long.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KeyBox_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_KeyBox_Long"),
		CUI_KeyBox_Long::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//====================================================================================================================================== ���� �ؽ�ó

	/* For.Prototype_Component_Texture_UI_ButtonHighlight*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ButtonHighlight"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_ButtonHighlight_02.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KeyBox_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ButtonHighlight"),
		CUI_ButtonHighlight::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================== ���� �ٹ� �ؽ�ó
	

	/* For.Prototype_Component_Texture_UI_DecorationLine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DecorationLine"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_DecorationLine_05.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KeyBox_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DecorationLine"),
		CUI_DecorationLine::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================== ���� �ؽ�ó

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

	//==================================================================================================================================== �÷��̾� �޴�â(��������)


		/* For.Prototype_Component_Texture_UI_HighlightBar_02*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HighlightBar_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_HighlightBar_02.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_HighlightBar_02 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HighlightBar_02"),
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
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/PlayerMeun/UI_LevelImage_Circus.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelImage */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LevelImage"),
		CUI_LevelImage::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	//==================================================================================================================================== �÷��̾� 

	/* For.Prototype_Component_Texture_ChairUI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ChairUI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/UI_%d.dds"), 8))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ChairUI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Slot_Attribute"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Slot_Attribute_%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MouseCursor*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_MouseCursor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/TexUI_GameCursor.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LevelUP"),
		CUI_LevelUP::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Attribute"),
		CUI_PlayerAttribute::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	///* For.Prototype_GameObject_UI_Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Component"),
	//	CUI_LeftBackground::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	/* For.Prototype_GameObject_UI_Component */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Slot_Attribute"),
		CUI_AttributeButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MouseCursor */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MouseCursor"),
		CUI_MouseCursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion


#pragma region Ƽ�޽þ� ĳ���� 
	lstrcpyW(m_szLoadingText, TEXT("Ƽ�޽þ� ���� �����Ѵ�."));	
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


#pragma region �׽�Ʈ�� ��ٸ�
	lstrcpyW(m_szLoadingText, TEXT("��ٸ� ���� �����Ѵ�."));
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
#pragma region Ƽ�޽þ� ����

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
	/* �ε��� �Ϸ�Ǿ������̴� */
	lstrcpyW(m_szLoadingText, TEXT("�ε���."));	
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
