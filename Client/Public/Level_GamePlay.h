#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
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
	HRESULT Ready_Layer_Structure(const _tchar* pLayerTag);	
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);	
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);

	HRESULT Ready_Layer_UI_Scene(const _tchar* pLayerTag);


	// 테스트용 사다리 레이어 추가
	HRESULT	Ready_Layer_Ladder(const _tchar* pLayerTag);
	
	//임시 UI 클릭 함수 - 유빈
	HRESULT Select_UI();
	HRESULT On_Mouse_UI();
	void ShowGUI(); // 임구이
	void SetUIScene();
	void SetUISetting();
	void SetUIImage();


	//ui 오브젝트 생성
	HRESULT Create_UI_Scene_Object();

	HRESULT Save_UI_IMGUI_Textrue();
	HRESULT Save_UI_Textrue(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);

	HRESULT SaveData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName); // 현재 UI 저장
	HRESULT LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName); // 현재 UI 저장
private:
	_uint m_iOpenSceneCount = {0};
	_bool m_bSceneOpen = { false };
private:
	_float3				m_fPos = {};			// UI 좌표, Z는 깊이 값
	_float2				m_fSize = {};			// UI 크기 값
	const _tchar*		m_szUIName = {};		// UI Prototype 값

	const _tchar*		m_szUISceneName = {};	// UI 씬 종류 (메뉴, 특성, 레벨 업..등등)
	_uint				m_iSceneIndex = {};		// 씬 번호
	
	_uint				m_iUIType = {0};			// UI 오브젝트 타입
	_uint				m_iShaderPassNum = {12};		// 쉐이더 패스
	
	_char*				m_szTypeName = {};		// IMGUI 기능 설명 용
	_int				m_iUIScenIndex = {0};	// IMGUI 기능 용

	
	_tchar*				m_szSaveName = {}; // 현재 쓰는 곳 없음

	map<_wstring, ID3D11ShaderResourceView*>				m_SRVs; // IMGUI 텍스처
	_uint													m_iCountSRVs = { 0 }; // 혹시 몰라 저장하느 이미지 개수 카운트

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END