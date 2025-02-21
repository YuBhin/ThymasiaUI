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


	// �׽�Ʈ�� ��ٸ� ���̾� �߰�
	HRESULT	Ready_Layer_Ladder(const _tchar* pLayerTag);
	
	//�ӽ� UI Ŭ�� �Լ� - ����
	HRESULT Select_UI();
	HRESULT On_Mouse_UI();
	void ShowGUI(); // �ӱ���
	void SetUIScene();
	void SetUISetting();
	void SetUIImage();


	//ui ������Ʈ ����
	HRESULT Create_UI_Scene_Object();

	HRESULT Save_UI_IMGUI_Textrue();
	HRESULT Save_UI_Textrue(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);

	HRESULT SaveData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName); // ���� UI ����
	HRESULT LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName); // ���� UI ����
private:
	_uint m_iOpenSceneCount = {0};
	_bool m_bSceneOpen = { false };
private:
	_float3				m_fPos = {};			// UI ��ǥ, Z�� ���� ��
	_float2				m_fSize = {};			// UI ũ�� ��
	const _tchar*		m_szUIName = {};		// UI Prototype ��

	const _tchar*		m_szUISceneName = {};	// UI �� ���� (�޴�, Ư��, ���� ��..���)
	_uint				m_iSceneIndex = {};		// �� ��ȣ
	
	_uint				m_iUIType = {0};			// UI ������Ʈ Ÿ��
	_uint				m_iShaderPassNum = {12};		// ���̴� �н�
	
	_char*				m_szTypeName = {};		// IMGUI ��� ���� ��
	_int				m_iUIScenIndex = {0};	// IMGUI ��� ��

	
	_tchar*				m_szSaveName = {}; // ���� ���� �� ����

	map<_wstring, ID3D11ShaderResourceView*>				m_SRVs; // IMGUI �ؽ�ó
	_uint													m_iCountSRVs = { 0 }; // Ȥ�� ���� �����ϴ� �̹��� ���� ī��Ʈ

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END