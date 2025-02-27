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
	struct UI_TextInfo // �ؽ�Ʈ ���� �����
	{
		_uint		iTextID;			// �ؽ�Ʈ ���� Ű��
		_wstring	strFontName;		// ����ϴ� ��Ʈ �̸�
		_wstring	srtTextContent;		// �ؽ�Ʈ ����
		_float2		fTextStartPos;		// �׷����� ������(����X)
		_float2		fTextSize;			// TextContent ���̿� ���� �ڵ����� �������� ���� ���� ����
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
	// UI ��
	HRESULT Ready_Layer_UI_Scene(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Ladder(const _tchar* pLayerTag);
	//�ӽ� UI Ŭ�� �Լ� - ����
	HRESULT Select_UI();
	HRESULT On_Mouse_Text();

	// �ӱ���
	void ShowGUI(); 
	void SetUIScene();
	void SetUIObj_Type();
	void SetUISetting();
	void SetUIImage();
	void SetUIFont();
	void SetUIFontName();

	//�ӱ��� ���ҽ� �߰�
	HRESULT Save_UI_IMGUI_Textrue();
	//�ӱ��� ���ҽ� �ε�
	HRESULT Save_UI_Textrue(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);
	//�ӱ��� ���콺 Ŭ�� ����
	//��ü ã�� �Լ�
	//ui ������Ʈ ����
	HRESULT Create_UI_Scene_Object();
	//����
	HRESULT SaveData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName); // ���� UI ����
	HRESULT LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName); // ���� UI ����
	//Font ���
	HRESULT Create_Text();
	//�ؽ�Ʈ ����
	HRESULT SaveData_Text_Scene();
	HRESULT LoadData_Text_Scene();

	HRESULT Delete_TextContainer_ALL();
	HRESULT Delete_TextContainer_Last();

private:
	_uint m_iOpenSceneCount = {0}; // �׽�Ʈ ��
	_bool m_bSceneOpen = { false };// �׽�Ʈ ��

private: // IMGUI ���ۿ� ��
	_bool m_bUIorText = { true }; // true == UI, false == Text

private: // UI ���� �����
	_float3				m_fPos = {};			// UI ��ǥ, Z�� ���� �� //  ���� ���� �� ����� ��
	_float2				m_fSize = {};			// UI ũ�� ��// ���� ���� �� ����� ��
	_float3				m_fRotation = {};			// UI ũ�� ��// ���� ���� �� ����� ��
	
	const _tchar*		m_szUIName = {};		// UI Prototype ��
	const _tchar*		m_szUISceneName = {};	// UI �� ���� (�޴�, Ư��, ���� ��..���)
	_uint				m_iSceneIndex = {};		// �� ��ȣ
	
	_uint				m_iUIType = {0};			// UI ������Ʈ Ÿ��
	_uint				m_iShaderPassNum = {12};		// ���̴� �н�
	_uint				m_iGroupID = {0};		// �׷���̵�
	_uint				m_iTexNumber = {0};		// �ؽ�ó �ѹ�
private: //IMGUI

	_char*				m_szTypeName = {};		// IMGUI ��� ���� ��
	_int				m_iUIScenIndex = {0};	// IMGUI ��� ��

	_bool				m_bGetUIObjCheck = { true }; // true  , false ��ü ���� �Ϸ� 
	_bool				m_bMouseMoveCheck = { false }; // ������ ui�� ���콺�� ���󰡰� �ϴ� ����

	CUIObject*			m_pCurrentUIObj = {}; // �ӱ��� ���ǿ� ���� �����ϱ� ���ؼ� ������� �߰�

	_tchar*				m_szSaveName = {}; // Imgui �ؽ�Ʈ �����ϴ� �κп��� �� �ߴ� �κ� �����ϱ� ���� ��� ������ ����

	map<_wstring, ID3D11ShaderResourceView*>				m_SRVs; // IMGUI �ؽ�ó
	_uint													m_iCountSRVs = { 0 }; // Ȥ�� ���� �����ϴ� �̹��� ���� ī��Ʈ

private: //Text Imgui
	const _tchar*		m_szFontName = L"Font_Gulim_Default";// ����ϴ� ��Ʈ �̸�  
	const _tchar*		m_szTextContent = L"�ȳ��ϼ���";    // ��µǴ� �ؽ�Ʈ ����

	_float2				TextTest = {100.f,100.f};

	_bool				m_bGetTextCheck = { true }; //
	_bool				m_bTexMouseOn = { false };
	_bool				m_bCurrentText = { false }; //���� ������ �ؽ�Ʈ ������ ������ 
	_uint				m_iTextID = {0};

	UI_TextInfo			m_CurrentText = {};
	vector<UI_TextInfo> m_TextInfo = {};
	//multimap<const _tchar*, _float2> m_TextInfo; //�ؽ�Ʈ ����� �ؽ�Ʈ ����� ���� SaveLoad �뵵

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END