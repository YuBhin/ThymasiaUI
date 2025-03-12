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
		_float4     fTextColor;         // �ؽ�Ʈ ���� ��������!
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
	HRESULT Ready_Layer_UIGroup_GameIntro(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerMenu(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerLevelUP(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_Iventory(const _tchar* pLayerTag);

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
	HRESULT Save_UI_Textrue_Share(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Save_UI_Textrue_Menu(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Save_UI_Textrue_LevelUp(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Save_UI_Textrue_Attribute(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Save_UI_Textrue_PlayerMainScreen(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Save_UI_Textrue_Inventory(const _tchar* _pObjTag, const _tchar* pTextureFilePath, _uint iNumTextures);

	//�ӱ��� ���콺 Ŭ�� ����
	//��ü ã�� �Լ�
	//ui ������Ʈ ����
	HRESULT Create_UI_Scene_Object();
	//����
	HRESULT SaveData_UI_Scene(_uint iSceneIndex/*, const _tchar* szSceneName*/); // ���� UI ����
	HRESULT LoadData_UI_Scene(_uint iSceneIndex/*, const _tchar* szSceneName*/); // ���� UI ����
	//Font ���
	HRESULT Create_Text();
	//�ؽ�Ʈ ����
	HRESULT SaveData_Text_Scene();
	HRESULT LoadData_Text_Scene();

	HRESULT Delete_TextContainer_ALL();
	HRESULT Delete_TextContainer_Last();
	HRESULT Delete_TextContainer_Choice();
private:
	// Imgui ���� �״�
	_bool m_bIMGUI = { true };// �׽�Ʈ ��


private:
	_uint m_iOpenSceneCount = {0}; // �׽�Ʈ ��
	_bool m_bSceneOpen = { false };// �׽�Ʈ ��

private: // IMGUI ���ۿ� ��
	_bool m_bUIorText = { true }; // true == UI, false == Text

private: // UI ���� �����
	_float3				m_fPos = {};			// UI ��ǥ, Z�� ���� �� //  ���� ���� �� ����� ��
	_float2				m_fSize = {};			// UI ũ�� ��// ���� ���� �� ����� ��
	_float3				m_fRotation = {};			// UI ũ�� ��// ���� ���� �� ����� ��
	
	const _tchar*		m_pUIName = {};		// UI Prototype ��
	const _tchar*		m_pUISceneName = {L"UIScene_Aditor"};	// UI �� ���� (�޴�, Ư��, ���� ��..���)
	_uint				m_iSceneIndex = { UISCENE_ALL };		// �� ��ȣ
	
	_uint				m_iUIType = {0};			// UI ������Ʈ Ÿ��
	_uint				m_iShaderPassNum = {1};		// ���̴� �н�
	_uint				m_iGroupID = {0};		// �׷���̵�
	_uint				m_iTexNumber = {0};		// �ؽ�ó �ѹ�
private: //IMGUI

	_char*				m_pTypeName = {};		// IMGUI ��� ���� ��
	_int				m_iUIScenIndex = {};	// IMGUI ��� ��

	_bool				m_bGetUIObjCheck = { true }; // true  , false ��ü ���� �Ϸ� 
	_bool				m_bMouseMoveCheck = { false }; // ������ ui�� ���콺�� ���󰡰� �ϴ� ����

	CUIObject*			m_pCurrentUIObj = {}; // �ӱ��� ���ǿ� ���� �����ϱ� ���ؼ� ������� �߰�

	_tchar*				m_pSaveName = {}; // Imgui �ؽ�Ʈ �����ϴ� �κп��� �� �ߴ� �κ� �����ϱ� ���� ��� ������ ����
	_char*				m_pUIScenePrint = {"UIScene_Default"}; // Imgui �ؽ�Ʈ �����ϴ� �κп��� �� �ߴ� �κ� �����ϱ� ���� ��� ������ ����
	map<_wstring, ID3D11ShaderResourceView*>				m_ShareSRVs; // IMGUI �ؽ�ó
	map<_wstring, ID3D11ShaderResourceView*>				m_MenuSRVs; // IMGUI �ؽ�ó
	map<_wstring, ID3D11ShaderResourceView*>				m_LevelUpSRVs; // IMGUI �ؽ�ó
	map<_wstring, ID3D11ShaderResourceView*>				m_AttributeSRVs; // IMGUI �ؽ�ó
	map<_wstring, ID3D11ShaderResourceView*>				m_PlayerSRVs; // IMGUI �ؽ�ó
	map<_wstring, ID3D11ShaderResourceView*>				m_InvenSRVs; // IMGUI �ؽ�ó

	_uint													m_iCountSRVs = { 0 }; // Ȥ�� ���� �����ϴ� �̹��� ���� ī��Ʈ

private: //Text Imgui
	const _tchar*		m_pFontName = L"Font_Gulim_Default";// ����ϴ� ��Ʈ �̸�  
	const _tchar*		m_pTextContent = L"�ȳ��ϼ���";    // ��µǴ� �ؽ�Ʈ ����

	_float2				TextTest = {100.f,100.f};

	_bool				m_bGetTextCheck = { true }; //
	_bool				m_bTexMouseOn = { false };
	_bool				m_bCurrentText = { false }; //���� ������ �ؽ�Ʈ ������ ������ 
	_uint				m_iTextID = {0};

	UI_TextInfo			m_CurrentText = {};
	vector<UI_TextInfo> m_TextInfo = {};
	//multimap<const _tchar*, _float2> m_TextInfo; //�ؽ�Ʈ ����� �ؽ�Ʈ ����� ���� SaveLoad �뵵

private:

	const _tchar*		m_pTextBoxFont = {L"Font_NotoSansKR16"};  // ui ������Ʈ �ؽ�Ʈ��
	_tchar*				m_pTextBoxContent = { L"UI �⺻ ��� ��" };


public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END