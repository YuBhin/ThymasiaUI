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
	HRESULT Ready_Layer_UI_Scene(const _tchar* pLayerTag);
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

	//임구이 리소스 추가
	HRESULT Save_UI_IMGUI_Textrue();
	//임구이 리소스 로드
	HRESULT Save_UI_Textrue(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);
	//임구이 마우스 클릭 제어
	//객체 찾는 함수
	//ui 오브젝트 생성
	HRESULT Create_UI_Scene_Object();

	//저장
	HRESULT SaveData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName); // 현재 UI 저장
	HRESULT LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName); // 현재 UI 저장

	//Font 출력
	HRESULT Create_Text();
	//텍스트 저장
	HRESULT SaveData_Text_Scene();
	HRESULT LoadData_Text_Scene();

	HRESULT Delete_TextContainer_ALL();

private:
	_uint m_iOpenSceneCount = {0};
	_bool m_bSceneOpen = { false };

private:
	_bool m_bUIorText = { true }; // true == UI, false == Text

private: // UI 정보 저장용
	_float3				m_fPos = {};			// UI 좌표, Z는 깊이 값 //  최초 생성 시 저장용 값
	_float2				m_fSize = {};			// UI 크기 값// 최초 생성 시 저장용 값
	_float3				m_fRotation = {};			// UI 크기 값// 최초 생성 시 저장용 값
	
	const _tchar*		m_szUIName = {};		// UI Prototype 값
	const _tchar*		m_szUISceneName = {};	// UI 씬 종류 (메뉴, 특성, 레벨 업..등등)
	_uint				m_iSceneIndex = {};		// 씬 번호
	
	_uint				m_iUIType = {0};			// UI 오브젝트 타입
	_uint				m_iShaderPassNum = {12};		// 쉐이더 패스
private: //IMGUI

	_char*				m_szTypeName = {};		// IMGUI 기능 설명 용
	_int				m_iUIScenIndex = {0};	// IMGUI 기능 용

	_bool				m_bGetUIObjCheck = { true }; // true  , false 객체 선택 완료 
	_bool				m_bMouseMoveCheck = { false }; // 선택한 ui가 마우스를 따라가게 하는 여부

	CUIObject*			m_pCurrentUIObj = {}; // 임구이 조건에 의해 저장하기 위해서 멤버변수 추가

	_tchar*				m_szSaveName = {}; // Imgui 텍스트 저장하는 부분에서 릭 뜨는 부분 설정하기 위해 멤버 변수로 만듬

	map<_wstring, ID3D11ShaderResourceView*>				m_SRVs; // IMGUI 텍스처
	_uint													m_iCountSRVs = { 0 }; // 혹시 몰라 저장하느 이미지 개수 카운트

private: //Text Imgui
	const _tchar*		m_szFontName = L"Font_Gulim_Default";// 사용하는 폰트 이름  
	const _tchar*		m_szTextContent = L"테스트 출력";    // 출력되는 텍스트 내용

	_float2				TextTest = {100.f,100.f};

	_bool				m_bGetTextCheck = { true }; //
	_bool				m_bTexMouseOn = { false };
	_bool				m_bCurrentText = { false }; //현재 선택한 텍스트 변경할 것인지 
	_uint				m_iTextID = {0};

	UI_TextInfo			m_CurrentText = {};
	vector<UI_TextInfo> m_TextInfo = {};
	//multimap<const _tchar*, _float2> m_TextInfo; //텍스트 내용과 텍스트 사이즈를 저장 SaveLoad 용도

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END