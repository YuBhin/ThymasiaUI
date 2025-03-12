#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CUI_Scene;
//class CUI_Text;
END

BEGIN(Client)

class CUIGroup_Iventory final : public CUIObject
{
public:
	struct UI_Item {

		_uint ItemID = {};
		_uint ItemIconNum = {};
		_uint ItemType = {};
		_tchar* ItemName = {};
		_tchar* ItemDesc = {};
		_uint ItemCount = {};
	};
	enum UIItemType {ITEM_COMMON,ITEM_DIRECT_USE, ITEM_NOUSE, ITEM_ONLYUSE,ITEM_SKILL,ITEM_END};

private:
	CUIGroup_Iventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_Iventory(const CUIGroup_Iventory& Prototype);
	virtual ~CUIGroup_Iventory() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_UIObject();

public:
	HRESULT Change_UI_Tab();
	HRESULT Change_UI_Item_Tab();

public:
	HRESULT Create_Item(_int iKeyNum); // �׽�Ʈ�� ������ ���� �Լ�

	HRESULT Item_Get(UI_Item ItemInfo); // ������ ����� �� �����̳ʿ� ����
	HRESULT Item_Get2(UI_Item ItemInfo); // ������ ����� �� �����̳ʿ� ����
	HRESULT Itme_View_Inventory(vector<UI_Item> &vecContainer); // �κ��丮 ���Կ� ������ �ִ� ������ ����
	//HRESULT InventoryUpdate(); // �κ��丮 ���� ������ ���� => �������� ��ų� ���, ���������� �����ϵ��� �� ����

private:
	_bool m_bCommonOpen = { true };
	_bool m_bSkillOpen = { false };



public:
	HRESULT Slot_Button_Select_Check(vector<UI_Item>& vecContainer);
	HRESULT Slot_Button_MouseOn_Check(vector<UI_Item>& vecContainer);

private:
	_uint m_iMouseOnLastSlot = {}; // ���������� ���콺�� ����Ų ������ �׷� ���̵�
	

public:
	HRESULT ItemType_PopUP_Open(_uint iItemType, _float3 fSetPos);
	HRESULT ItemType_PopUP_State(_bool bOpen); // �ܼ� ���� �Ѱ�
	HRESULT ItemType_PopUP_Button(); //�˾��� �ִ� ��ư ������ ���� ó��

	HRESULT ItemUse_PopUP_Open(); //
	HRESULT ItemUse_PopUP_Button();

private:
	_bool m_bItemUsePopOpen = { false }; // 2��° �˾��� ���� ��� �˾��� �����ϱ� �������� �ٸ� �۾��� ���� ����
	_bool m_bItemTypePopOpen = { false }; // 1��° �˾��� ���� ��� ���� ������ �ٽ� �޾ƿ��� ����

private:
	CUI_Scene* m_pMyBaseScene = {}; // Base ���
	CUI_Scene* m_pItemScene = {}; // UI����
	CUI_Scene* m_pItemTypePopUp = {}; // 1 �� �˾�
	CUI_Scene* m_pItemUsePopUp = {}; // 2 �� �˾�

	vector<UI_TextInfo> m_TextInfo = {};

	vector<UI_Item> m_InvenItem = {}; // �Ϲ� ������ => �׽�Ʈ ������ ����°� ���߿� ������ �Ŵ������� �����̳ʸ� �޾� ���� ����
	vector<UI_Item> m_InvenItemSkill= {}; // ����� ���� => 
	
	
	_bool m_bInvenCheck = { false }; // ���� �������� �ִ� �� üũ�ϴ� �κ�
	_bool m_bSkillItemCheck = { false }; // ���� �������� �ִ� �� üũ�ϴ� �κ�

public:
	HRESULT Ready_MiniView_ItemInfo(); // �ݰ��� ���� �ٲ�� �ؽ�Ʈ�� �׳� ���ϰ� ������ �ֱ�
	HRESULT Connect_MiniView_ItemInfo(UI_Item ItemInfo); // �̸� �ع� �ص� �ؽ�Ʈ �ڽ� ������ ����
	HRESULT DisConnect_MiniView_ItemInfo(); // ������ ���� ���� �� �� ���� ���

private:
	CGameObject* m_pItemTextBoxName = {};		//200 �̸�
	CGameObject* m_pItemTextBoxDesc = {};		//230 ����
	CGameObject* m_pItemTextBoxType = {};		//210 Ÿ�� �ؽ�Ʈ
	CGameObject* m_pItemTextBoxCount = {};		//220 ���� �ؽ�Ʈ
	CGameObject* m_pItemTextBoxCountNum = {};	//221 n/n ����
	CGameObject* m_pCurrentButton = {};

public:
	static CUIGroup_Iventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

