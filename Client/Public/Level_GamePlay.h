#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CUIObject;
END
BEGIN(Client)


class CLevel_GamePlay final : public CLevel
{
public:
	struct UI_TextInfo // 텍스트 정보 저장용
	{
		_uint		iTextID;			// 텍스트 고유 키값
		_wstring	strFontName;		// 사용하는 폰트 이름
		_wstring	srtTextContent;		// 텍스트 내용
		_float2		fTextStartPos;		// 그려지는 시작점(중점X)
		_float2		fTextSize;			// TextContent 길이에 따라 자동으로 구해지는 가로 세로 길이
		_float4     fTextColor;         // 텍스트 색상 저장하자!
	};

private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);	
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	// UI 씬
	HRESULT Ready_Layer_UIGroup_GameIntro(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerMenu(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerLevelUP(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_Iventory(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Ladder(const _tchar* pLayerTag);
	//임시 UI 클릭 함수 - 유빈
	HRESULT Select_UI();
	HRESULT On_Mouse_Text();

	// 임구이
	void ShowGUI(); 
	void SetUIScene();
	void SetUIObj_Type();
	void SetUISetting();
	void SetUIImage();
	void SetUIFont();
	void SetUIFontName();

	//임구이 리소스 추가
	HRESULT Save_UI_IMGUI_Textrue();
	//임구이 리소스 로드
	HRESULT Save_UI_Textrue_Share(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Save_UI_Textrue_Menu(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Save_UI_Textrue_LevelUp(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Save_UI_Textrue_Attribute(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Save_UI_Textrue_PlayerMainScreen(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Save_UI_Textrue_Inventory(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);

	//임구이 마우스 클릭 제어
	//객체 찾는 함수
	//ui 오브젝트 생성
	HRESULT Create_UI_Scene_Object();
	//저장
	HRESULT SaveData_UI_Scene(_uint iSceneIndex/*, const _tchar* szSceneName*/); // 현재 UI 저장
	HRESULT LoadData_UI_Scene(_uint iSceneIndex/*, const _tchar* szSceneName*/); // 현재 UI 저장
	//Font 출력
	HRESULT Create_Text();
	//텍스트 저장
	HRESULT SaveData_Text_Scene();
	HRESULT LoadData_Text_Scene();

	HRESULT Delete_TextContainer_ALL();
	HRESULT Delete_TextContainer_Last();
	HRESULT Delete_TextContainer_Choice();
private:
	// Imgui 껐다 켰다
	_bool m_bIMGUI = { true };// 테스트 용


private:
	_uint m_iOpenSceneCount = {0}; // 테스트 용
	_bool m_bSceneOpen = { false };// 테스트 용

private: // IMGUI 조작용 값
	_bool m_bUIorText = { true }; // true == UI, false == Text

private: // UI 정보 저장용
	_float3				m_fPos = {};			// UI 좌표, Z는 깊이 값 //  최초 생성 시 저장용 값
	_float2				m_fSize = {};			// UI 크기 값// 최초 생성 시 저장용 값
	_float3				m_fRotation = {};			// UI 크기 값// 최초 생성 시 저장용 값
	
	const _tchar*		m_pUIName = {};		// UI Prototype 값
	const _tchar*		m_pUISceneName = {L"UIScene_Aditor"};	// UI 씬 종류 (메뉴, 특성, 레벨 업..등등)
	_uint				m_iSceneIndex = { UISCENE_ALL };		// 씬 번호
	
	_uint				m_iUIType = {0};			// UI 오브젝트 타입
	_uint				m_iShaderPassNum = {1};		// 쉐이더 패스
	_uint				m_iGroupID = {0};		// 그룹아이디
	_uint				m_iTexNumber = {0};		// 텍스처 넘버
private: //IMGUI

	_char*				m_pTypeName = {};		// IMGUI 기능 설명 용
	_int				m_iUIScenIndex = {};	// IMGUI 기능 용

	_bool				m_bGetUIObjCheck = { true }; // true  , false 객체 선택 완료 
	_bool				m_bMouseMoveCheck = { false }; // 선택한 ui가 마우스를 따라가게 하는 여부

	CUIObject*			m_pCurrentUIObj = {}; // 임구이 조건에 의해 저장하기 위해서 멤버변수 추가

	_tchar*				m_pSaveName = {}; // Imgui 텍스트 저장하는 부분에서 릭 뜨는 부분 설정하기 위해 멤버 변수로 만듬
	_char*				m_pUIScenePrint = {"UIScene_Default"}; // Imgui 텍스트 저장하는 부분에서 릭 뜨는 부분 설정하기 위해 멤버 변수로 만듬
	map<_wstring, ID3D11ShaderResourceView*>				m_ShareSRVs; // IMGUI 텍스처
	map<_wstring, ID3D11ShaderResourceView*>				m_MenuSRVs; // IMGUI 텍스처
	map<_wstring, ID3D11ShaderResourceView*>				m_LevelUpSRVs; // IMGUI 텍스처
	map<_wstring, ID3D11ShaderResourceView*>				m_AttributeSRVs; // IMGUI 텍스처
	map<_wstring, ID3D11ShaderResourceView*>				m_PlayerSRVs; // IMGUI 텍스처
	map<_wstring, ID3D11ShaderResourceView*>				m_InvenSRVs; // IMGUI 텍스처

	_uint													m_iCountSRVs = { 0 }; // 혹시 몰라 저장하느 이미지 개수 카운트

private: //Text Imgui
	const _tchar*		m_pFontName = L"Font_Gulim_Default";// 사용하는 폰트 이름  
	const _tchar*		m_pTextContent = L"안녕하세요";    // 출력되는 텍스트 내용

	_float2				TextTest = {100.f,100.f};

	_bool				m_bGetTextCheck = { true }; //
	_bool				m_bTexMouseOn = { false };
	_bool				m_bCurrentText = { false }; //현재 선택한 텍스트 변경할 것인지 
	_uint				m_iTextID = {0};

	UI_TextInfo			m_CurrentText = {};
	vector<UI_TextInfo> m_TextInfo = {};
	//multimap<const _tchar*, _float2> m_TextInfo; //텍스트 내용과 텍스트 사이즈를 저장 SaveLoad 용도

private:

	const _tchar*		m_pTextBoxFont = {L"Font_NotoSansKR16"};  // ui 오브젝트 텍스트용
	_tchar*				m_pTextBoxContent = { L"UI 기본 출력 값" };


public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END