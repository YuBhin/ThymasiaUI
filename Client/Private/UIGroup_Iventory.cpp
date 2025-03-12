#include "pch.h"
#include "UIGroup_Iventory.h"
#include "UI_Scene.h"
#include "UI_Button.h"
#include "UI_Text.h"
#include "UI_Image.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "UI_ItemBackground.h"
#include "UI_Item_Icon.h"
#include "UI_ButtonHighlight.h"
#include "UI_SquareFrame.h"
#include "UI_SquareFrame_Hover.h"
#include "UI_UnderLine.h"

CUIGroup_Iventory::CUIGroup_Iventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_Iventory::CUIGroup_Iventory(const CUIGroup_Iventory& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_Iventory::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_Iventory::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	m_pMyBaseScene = m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase");
	m_pItemScene = m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_Inven_ItemUse");

	// ���� ������ �ʿ��� ui�� ���ѿ��� off �ϵ��� ����
	m_pItemUsePopUp = m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_ItemUse_PopUp");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItemUsePopUp, false);

	m_pItemTypePopUp = m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_ItemType_PopUp");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItemTypePopUp, false);

	if (FAILED(Ready_MiniView_ItemInfo()))
		return E_FAIL;

	return S_OK;
}

void CUIGroup_Iventory::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	 //�׷쿡�� ���� ������ �Ǵ� ���� ������ ��� �ٸ� �͵鵵 �������� => �ش� ������ ������Ʈ�� �Ҵٴ� ��
	if (m_pMyBaseScene->Get_Scene_Render_State())
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemScene, true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemUsePopUp,true); // �ڵ����� uiobj�� ���� scene ������ ���µǵ��� �Լ��� ����
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemTypePopUp, true);
	}
	else
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemScene, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemUsePopUp, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemTypePopUp, false);
		m_iMouseOnLastSlot = 0; // ���� ������ �� �ʱ�ȭ
	}

}

void CUIGroup_Iventory::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	//if (m_bRenderOpen) // 
	//	m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMyScene, true);
	if (!m_bItemUsePopOpen)
	{
		Change_UI_Item_Tab();

		if (!m_bItemTypePopOpen)
		{
			if (m_pGameInstance->isKeyEnter(DIK_1))
			{
				Create_Item(1);
			}
			if (m_pGameInstance->isKeyEnter(DIK_2))
			{
				Create_Item(2);
			}
			if (m_pGameInstance->isKeyEnter(DIK_3))
			{
				Create_Item(3);
			}
			if (m_pGameInstance->isKeyEnter(DIK_4))
			{
				Create_Item(4);
			}
			if (m_pGameInstance->isKeyEnter(DIK_5))
			{
				Create_Item(5);
			}
			if (m_pGameInstance->isKeyEnter(DIK_6))
			{
				Create_Item(6);
			}
			if(m_bCommonOpen)
				Slot_Button_MouseOn_Check(m_InvenItem);
			if (m_bSkillOpen)
				Slot_Button_MouseOn_Check(m_InvenItemSkill);

		}
		// ���콺 On ���� true �� �༮�� ã�� ���� ���� ����

		if (m_bCommonOpen)
			Slot_Button_Select_Check(m_InvenItem);
		if (m_bSkillOpen)
			Slot_Button_Select_Check(m_InvenItemSkill);
		ItemType_PopUP_Button();
	}
	else
	{
		ItemUse_PopUP_Button();
	}

	
	 
	
}

void CUIGroup_Iventory::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);


	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUIGroup_Iventory::Render()
{
	if (m_bRenderOpen)
	{
		
	}
	return S_OK;
}

HRESULT CUIGroup_Iventory::Ready_UIObject()
{
	// ESC �޴� ���
	if (FAILED(m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_GAMEPLAY, UISCENE_INVEN, L"UIScene_EscMenuBase")))
		return E_FAIL;

	// �κ� ������ ���� ����
	if (FAILED(m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_GAMEPLAY, UISCENE_INVEN, L"UIScene_Inven_ItemUse")))
		return E_FAIL;

	// �κ� ������ ��� ������ ���� �˾�
	if (FAILED(m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_GAMEPLAY, UISCENE_INVEN, L"UIScene_ItemUse_PopUp")))
		return E_FAIL;
	//�κ� ������ Ÿ�Կ� ���� �˾�
	if (FAILED(m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_GAMEPLAY, UISCENE_INVEN, L"UIScene_ItemType_PopUp")))
		return E_FAIL;
	//m_pGameInstance->LoadDataFile_UIText_Info(g_hWnd, L"UIScene_PlayerScreen", m_TextInfo);

	return S_OK;
}

HRESULT CUIGroup_Iventory::Change_UI_Tab()
{
	return S_OK;
}

HRESULT CUIGroup_Iventory::Change_UI_Item_Tab()
{
	for (auto& Button : m_pItemScene->Find_UI_Button())
	{
		CUI_UnderLine* pButton = dynamic_cast<CUI_UnderLine*>(Button);

		if (10 == Button->Get_UI_GroupID()) // �Ϲ� 
		{
			if (pButton->Get_Mouse_Select_OnOff()) // �Ϲ� ��ư�� ���� ��
			{
				m_bCommonOpen = true;
				m_bSkillOpen = false;
				/*if (m_InvenItem.empty())
				{
					DisConnect_MiniView_ItemInfo();
				}*/
				Itme_View_Inventory(m_InvenItem);
			}

		}
		if (20 == Button->Get_UI_GroupID()) // �̾߱�
		{

		}
		if (30 == Button->Get_UI_GroupID()) // ����� ����
		{
			if (pButton->Get_Mouse_Select_OnOff()) // ����� ���� ��ư�� ���� ��
			{
				m_bCommonOpen = false;
				m_bSkillOpen = true;




			/*	if (m_InvenItemSkill.empty())
				{
					DisConnect_MiniView_ItemInfo();
				}*/
					
				Itme_View_Inventory(m_InvenItemSkill);
			}
		}
		if (40 == Button->Get_UI_GroupID()) // ���
		{

		}

	}


	return S_OK;
}

HRESULT CUIGroup_Iventory::Ready_MiniView_ItemInfo()
{
	// 
	for (auto& TextBox : m_pItemScene->Find_UI_TextBox())
	{
		_tchar* pTemp = { L"" };
		if (200 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxName = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxName)->Set_Content(pTemp);
		}
		if (210 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxType = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxType)->Set_Content(pTemp);
		}
		if (220 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxCount = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxCount)->Set_Content(pTemp);
		}
		if (221 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxCountNum = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxCountNum)->Set_Content(pTemp);
		}

		if (230 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxDesc = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxDesc)->Set_Content(pTemp);
		}
		if (nullptr != m_pItemTextBoxName &&
			nullptr != m_pItemTextBoxDesc)
		{
			break;
		}
	}

	for (auto& image : m_pItemScene->Find_UI_Image())
	{
		if (200 <= image->Get_UI_GroupID() && 250 >= image->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Image*>(image)->Set_OnOff(false);
		}

	}
	return S_OK;
}

HRESULT CUIGroup_Iventory::Create_Item(_int iKeyNum)
{
	//�ӽ� ������ ���� �Լ� 
	UI_Item test = {};

	switch (iKeyNum)
	{
	case 1:
		test.ItemID = 10;
		test.ItemIconNum = 1;
		test.ItemType = ITEM_COMMON;
		test.ItemName = L"�츣�޽� �ձ� ����";
		test.ItemDesc = L"����� ���� ����� �� �� �ִ�";
		test.ItemCount = 1;
		Item_Get(test);
		break;

	case 2:
		test.ItemID = 20;
		test.ItemType = ITEM_DIRECT_USE;
		test.ItemIconNum = 5;
		test.ItemName = L"����� ����";
		test.ItemDesc = L"���� ���� �Ҹ�Ǵ� ��ȭ";
		test.ItemCount = 1;
		Item_Get(test);
		break;

	case 3:
		test.ItemID = 30;
		test.ItemType = ITEM_NOUSE;
		test.ItemIconNum = 4;
		test.ItemName = L"�������� ����";
		test.ItemDesc = L"��� �ɷ�ġ�� �ʱ�ȭ�ȴ�";
		test.ItemCount = 1;
		Item_Get(test);
		break;
	case 4:
		test.ItemID = 40;
		test.ItemType = ITEM_SKILL;
		test.ItemIconNum = 7;
		test.ItemName = L"��ġ";
		test.ItemDesc = L"����� ������ ����� �����Ͽ� ��ȣ�⿡�� �������⸦ �����ϰų�\n���׷��̵� �ϼ���.";
		test.ItemCount = 1;
		Item_Get2(test);
		break;
	case 5:
		test.ItemID = 50;
		test.ItemType = ITEM_SKILL;
		test.ItemIconNum = 7;
		test.ItemName = L"�ܵ�";
		test.ItemDesc = L"����� ������ ����� �����Ͽ� ��ȣ�⿡�� �������⸦ �����ϰų�\n���׷��̵� �ϼ���.";
		test.ItemCount = 1;
		Item_Get2(test);
		break;
	case 6:
		test.ItemID = 60;
		test.ItemType = ITEM_SKILL;
		test.ItemIconNum = 7;
		test.ItemName = L"�յ���";
		test.ItemDesc = L"����� ������ ����� �����Ͽ� ��ȣ�⿡�� �������⸦ �����ϰų�\n���׷��̵� �ϼ���.";
		test.ItemCount = 1;
		Item_Get2(test);
		break;
	}

	return S_OK;
}

HRESULT CUIGroup_Iventory::Item_Get(UI_Item ItemInfo) // ������ �Ŵ����� �Ұ� 
{
	for (auto& Item : m_InvenItem)
	{
		if (Item.ItemID == ItemInfo.ItemID) // �̹� �������� �ִٸ�
		{
			// ������ ���� ����
			Item.ItemCount++; // 99�� ������ ���⼭ �ϸ� �� �� ������....�ϴ� ����

			m_bInvenCheck = true; // ������ �������� �־��ٸ� üũ
			break; // ã������ �ݺ����� �����
		}
		else
		{
			// �����̳ʿ� ȹ���� �����۰� �����Ѱ� ������ üũ
			m_bInvenCheck = false;
		}
	}

	if (!m_bInvenCheck) // �����̳ʿ� �������� ���ٸ� ���Ӱ� �����Ѵ�
	{
		m_InvenItem.push_back(ItemInfo);
	}
	
	if(m_bCommonOpen) // ��� �κ��� ���� �� �������� ���� ���� ��� �ʿ� ���� �ڵ��̱� �� => �׽�Ʈ������ �߰��� ��
		Itme_View_Inventory(m_InvenItem); // �κ� ���Կ� ������ ������ �����Ѵ�

	return S_OK;
}

HRESULT CUIGroup_Iventory::Item_Get2(UI_Item ItemInfo)
{
	for (auto& Item : m_InvenItemSkill)
	{
		if (Item.ItemID == ItemInfo.ItemID) // �̹� �������� �ִٸ�
		{
			// ������ ���� ����
			Item.ItemCount++; // 99�� ������ ���⼭ �ϸ� �� �� ������....�ϴ� ����

			m_bSkillItemCheck = true; // ������ �������� �־��ٸ� üũ
			break; // ã������ �ݺ����� �����
		}
		else
		{
			// �����̳ʿ� ȹ���� �����۰� �����Ѱ� ������ üũ
			m_bSkillItemCheck = false;
		}
	}

	if (!m_bSkillItemCheck) // �����̳ʿ� �������� ���ٸ� ���Ӱ� �����Ѵ�
	{
		m_InvenItemSkill.push_back(ItemInfo);
	}

	if (m_bSkillOpen)
		Itme_View_Inventory(m_InvenItemSkill); // �κ� ���Կ� ������ ������ �����Ѵ�

	return S_OK;
}

HRESULT CUIGroup_Iventory::Itme_View_Inventory(vector<UI_Item>& vecContainer)
{
	// ������ ������ ������ �����Ѵ�
	sort(vecContainer.begin(), vecContainer.end(), [](UI_Item a, UI_Item b) {return a.ItemID < b.ItemID; }); // �������� ����

	// �κ��丮�� ȹ���� �����۵��� �����ش�

	_int iOpenSlotCount = 100; // ���� ��ȣ
		for (auto& InvenItem : vecContainer) // ������ ���� ���鼭 ���Կ� �ֱ�
		{
			for (auto& Slot : m_pItemScene->Find_UI_Button()) // ���� ��������
			{
				if (iOpenSlotCount == Slot->Get_UI_GroupID()) // ù ��° ���Ժ��� ���� ä���
				{
					dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Item_Icon(InvenItem.ItemIconNum);
					dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Item_Icon_OnOff(true);
					
					_tchar ChangeText[MAX_PATH] = {};
					const _tchar* Text = { L"x%d" };

					wsprintf(ChangeText, Text, InvenItem.ItemCount);
					dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Content(ChangeText);
					iOpenSlotCount++;
					break;
				}
			}

		}
		// ������ �����̳� ������ ���� �� ��� �� �������� �������� ����
		iOpenSlotCount = 100;
		if (vecContainer.empty())
		{
			for (auto& Slot : m_pItemScene->Find_UI_Button()) // ���� ��������
			{
				if (iOpenSlotCount == Slot->Get_UI_GroupID()) // ù ��° ���Ժ��� ���� ä���
				{
					_tchar ChangeText[MAX_PATH] = {L""};

					dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Item_Icon_OnOff(false);
					dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Content(ChangeText);
					iOpenSlotCount++;

				}
			}
		}
	//if (!m_InvenItem.empty())
	//	Connect_TextBox_ItemInfo(m_InvenItem.front());

	//// ���� Ȯ�ο� �����
	//for (auto& Item : m_InvenItem)
	//	std::cout << Item.ItemID << endl;

	return S_OK;
}
HRESULT CUIGroup_Iventory::Slot_Button_Select_Check(vector<UI_Item>& vecContainer)
{
	for (auto& Button : m_pItemScene->Find_UI_Button())
	{
		if (100 <= Button->Get_UI_GroupID() && 125 > Button->Get_UI_GroupID()) // slotbutton
		{
			CUI_ItemBackground* pButton = dynamic_cast<CUI_ItemBackground*>(Button);
			if (pButton->Get_Mouse_Select_OnOff())
			{
				// ��ư�� �� ���콺�� ������ �༮�� �ִ�!
				pButton->Set_Mouse_Select_OnOff(false);
				m_pCurrentButton = Button; 
				for (auto& Item : vecContainer)
				{
					if (Item.ItemIconNum == pButton->Get_Item_Icon())
					{
						_float3 ButtonPos = dynamic_cast<CTransform*>(pButton->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
						ItemType_PopUP_Open(Item.ItemType, ButtonPos);
						break;
					}
				}
			}
		}
	}




	return S_OK;
}
HRESULT CUIGroup_Iventory::Slot_Button_MouseOn_Check(vector<UI_Item>& vecContainer)
{
	for (auto& Slot : m_pItemScene->Find_UI_Button())
	{
		CUI_ItemBackground* pSlot = dynamic_cast<CUI_ItemBackground*>(Slot);

		if (100 <= Slot->Get_UI_GroupID() && 125 > Slot->Get_UI_GroupID())
		{
			if (pSlot->Get_Slot_Mouse_OnOff()) // �ش� ���Կ� ���콺�� �÷��� �ִ�
			{
				for (auto& Image : m_pItemScene->Find_UI_Image())
				{
					if (100 == Image->Get_UI_GroupID()) // ȣ�� �̹����� ã�Ƽ�
					{
						_float3 fPos = dynamic_cast<CTransform*>(Slot->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);

						CUI_SquareFrame_Hover* pImage = dynamic_cast<CUI_SquareFrame_Hover*>(Image);
						dynamic_cast<CTransform*>(pImage->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fPos.x,fPos.y });
						pImage->Set_Hover_ImageOn(true);
						//m_iMouseOnLastSlot = pSlot->Get_UI_GroupID();
					}
				}

				if (pSlot->Get_Item_Icon_OnOff()) // ���Կ� �������� �������� �ִ���
				{
					for (auto& Item : vecContainer)
					{
						if (Item.ItemIconNum == pSlot->Get_Item_Icon()) // ���Կ� �ִ� �������� �̹����� ���� ������ ���� ��������
							Connect_MiniView_ItemInfo(Item); // ui�� ����
					}
				}
				else
				{
					DisConnect_MiniView_ItemInfo();
				}

			}

		}

	}


	//for (auto& Slot : m_pItemScene->Find_UI_Button())
	//{
	//	if (m_iMouseOnLastSlot == Slot->Get_UI_GroupID())
	//	{
	//		if (dynamic_cast<CUI_ItemBackground*>(Slot)->Get_Item_Icon_OnOff()) // ���Կ� �������� �������� �ִ���
	//		{
	//			for (auto& Item : vecContainer)
	//			{
	//				if (Item.ItemIconNum == dynamic_cast<CUI_ItemBackground*>(Slot)->Get_Item_Icon()) // ���Կ� �ִ� �������� �̹����� ���� ������ ���� ��������
	//					Connect_MiniView_ItemInfo(Item); // ui�� ����
	//			}
	//		}
	//		else
	//		{
	//			DisConnect_MiniView_ItemInfo();
	//		}

	//	}
	//}
	return S_OK;
}

HRESULT CUIGroup_Iventory::ItemType_PopUP_Open(_uint iItemType, _float3 fSetPos)
{
	_float3 fImagePos = {};

	switch (iItemType)
	{
	case 0: //ITEM_COMMON // �ƹ��͵� �� ��
		ItemType_PopUP_State(false);
		m_bItemTypePopOpen = false;
		break;
	case 1: //ITEM_DIRECT_USE // ��� ���Ǵ� ������ // ��� ������ ���� ��
		for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
		{
			if (300 == Image->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fSetPos.x + 125,fSetPos.y + 75 });
				fImagePos = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
				Image->Set_OnOff(true);
			}
			if (400 == Image->Get_UI_GroupID())
			{
				Image->Set_OnOff(true);
			}
		}
		for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
		{
			if (310 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y - 40.f });
				Button->Set_OnOff(true);
			}
			if (320 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y });
				Button->Set_OnOff(true);
			}
			if (330 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y + 40.f });
				Button->Set_OnOff(true);
			}
		}
		m_bItemTypePopOpen = true;
		break;
	case 2: //ITEM_NOUSE // ������, ���� ��
		for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
		{
			if (300 == Image->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fSetPos.x + 125,fSetPos.y + 75 });
				fImagePos = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
				Image->Set_OnOff(true);
			}
			if (400 == Image->Get_UI_GroupID())
			{
				Image->Set_OnOff(true);
			}
		}
		for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
		{
			if (310 == Button->Get_UI_GroupID())
			{
				Button->Set_OnOff(false);
			}
			if (320 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y - 40.f });
				Button->Set_OnOff(true);
			}
			if (330 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y });
				Button->Set_OnOff(true);
			}
		}
		m_bItemTypePopOpen = true;
		break;

	case 3: //ITEM_ONLYUSE // ����ϱ�, ���� �� // �� ��ȣ��� ���� Į ������
		for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
		{
			if (300 == Image->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fSetPos.x + 125,fSetPos.y + 75 });
				fImagePos = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
				Image->Set_OnOff(true);
			}
			if (400 == Image->Get_UI_GroupID())
			{
				Image->Set_OnOff(true);
			}
		}
		for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
		{
			if (310 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y - 40.f });
				Button->Set_OnOff(true);
			}
			if (320 == Button->Get_UI_GroupID())
			{
				Button->Set_OnOff(false);
			}
			if (330 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y });
				Button->Set_OnOff(true);
			}
		}
		m_bItemTypePopOpen = true;
		break;
	case 4: //����� ���� ������ �����ϱ� ����
		for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
		{
			if (300 == Image->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fSetPos.x + 125,fSetPos.y + 75 });
				fImagePos = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
				Image->Set_OnOff(true);
			}
			if (400 == Image->Get_UI_GroupID())
			{
				Image->Set_OnOff(true);
			}
		}
		for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
		{
			if (310 == Button->Get_UI_GroupID())
			{
				Button->Set_OnOff(false);
			}
			if (320 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y - 40.f });
				Button->Set_OnOff(true);
			}
			if (330 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y });
				Button->Set_OnOff(true);
			}
		}
		m_bItemTypePopOpen = true;
		break;
	default: //�󽽷�
		ItemType_PopUP_State(false);
		m_bItemTypePopOpen = false;

		break;
	}




	return S_OK;
}
HRESULT CUIGroup_Iventory::ItemType_PopUP_State(_bool bOpen)
{

	for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
	{
		if (300 == Image->Get_UI_GroupID())
		{
			Image->Set_OnOff(bOpen);
		}
		if (400 == Image->Get_UI_GroupID())
		{
			Image->Set_OnOff(bOpen);
		}
	}
	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
	{
		if (310 == Button->Get_UI_GroupID())
		{
			Button->Set_OnOff(bOpen);
		}
		if (320 == Button->Get_UI_GroupID())
		{
			Button->Set_OnOff(bOpen);
		}
		if (330 == Button->Get_UI_GroupID())
		{
			Button->Set_OnOff(bOpen);
		}
	}

	return S_OK;
}

HRESULT CUIGroup_Iventory::ItemType_PopUP_Button()
{
	// �˾� ���� ���� => �̹��� �ٱ� ����
	for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
	{
		if (300 == Image->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_SquareFrame*>(Image)->Get_Mouse_Side_Select_OnOff())
			{
				ItemType_PopUP_State(false);
				m_bItemTypePopOpen = false;
			}
		}
	}
	// �˾� ���� ���� => ���� ��ư Ŭ��
	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
	{
 		if (330 == Button->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				ItemType_PopUP_State(false);
				m_bItemTypePopOpen = false;
			}
		}
	}

	// ��� ��ư Ŭ��
	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
	{
 		if (310 == Button->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				ItemUse_PopUP_Open();
			}
		}
	}


	

	return S_OK;
}

HRESULT CUIGroup_Iventory::ItemUse_PopUP_Open()
{
	_float3 fImagePos = {};

	//// ������ ���ǿ� ���� �ٲ�� �ϴµ� �������� �ָ��ؼ� �켱 ����Ʈ�� �����

	for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
	{
		Image->Set_OnOff(true);
	}

	for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
	{
		Button->Set_OnOff(true);
	}

	for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
	{
		TextBox->Set_OnOff(true);
	}


	m_bItemUsePopOpen = true;
	

	return S_OK;
}

HRESULT CUIGroup_Iventory::ItemUse_PopUP_Button()
{
	for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
	{
		if (30 == Button->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				ItemType_PopUP_State(false);
				m_bItemTypePopOpen = false;
				m_bItemUsePopOpen = false;
				for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
				{
					Image->Set_OnOff(false);
				}

				for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
				{
					Button->Set_OnOff(false);
				}

				for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
				{
					TextBox->Set_OnOff(false);
				}
			}
		}
		if (40 == Button->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_bItemUsePopOpen = false;
				for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
				{
					Image->Set_OnOff(false);
				}

				for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
				{
					Button->Set_OnOff(false);
				}

				for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
				{
					TextBox->Set_OnOff(false);
				}

			}
		}
	}
	return S_OK;
}

//
//HRESULT CUIGroup_Iventory::InventoryUpdate()
//{
//	for(auto& Slot : m_pMyScene->Find_UI_Button())
//	{
//		if (100 <= Slot->Get_UI_GroupID() && 125 > Slot->Get_UI_GroupID())
//		{
//			if (dynamic_cast<CUI_ItemBackground*>(Slot)->Get_Item_Icon_OnOff())
//			{
//				for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
//				{
//					if(Slot->Get_UI_GroupID() == TextBox->Get_UI_GroupID())
//						TextBox->Set_Render_OnOff(true);
//					break;
//				}
//			}
//			else // ���߿� ������ ���� �� ����
//			{
//				for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
//				{
//					if (Slot->Get_UI_GroupID() == TextBox->Get_UI_GroupID())
//						TextBox->Set_Render_OnOff(false);
//				}
//			}
//		}
//	}
//
//	return S_OK;
//}

HRESULT CUIGroup_Iventory::Connect_MiniView_ItemInfo(UI_Item ItemInfo)
{
	_tchar* pTemp = {};

	// ���콺�� �ö� ���� �� �ش� �������� ������ ��������
	// �̸� ������ ����
	
	//�ؽ�Ʈ �ڽ� n���� ������ �̸� ���
	dynamic_cast<CUIObject*>(m_pItemTextBoxName)->Set_Content(ItemInfo.ItemName);
	dynamic_cast<CUIObject*>(m_pItemTextBoxDesc)->Set_Content(ItemInfo.ItemDesc);

	_tchar* pSor = {L"%d"};
	_tchar pDust[MAX_PATH] = {};
	wsprintf(pDust, pSor, ItemInfo.ItemCount);
	pTemp = L"����";
	dynamic_cast<CUIObject*>(m_pItemTextBoxCount)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxCountNum)->Set_Content(pDust);

	switch(ItemInfo.ItemType)
	{
	case ITEM_COMMON:
		pTemp = L"�Ϲ�";
		break;
	case ITEM_DIRECT_USE:
		pTemp = L"�Һ� ������";
		break;
	case ITEM_NOUSE:
		pTemp = L"����ϱ� ����";
		break;
	case ITEM_ONLYUSE:
		pTemp = L"������ ����";
		break;
	case ITEM_SKILL:
		pTemp = L"����� ����";
		break;
	}
	dynamic_cast<CUIObject*>(m_pItemTextBoxType)->Set_Content(pTemp);

	for (auto& image : m_pItemScene->Find_UI_Image())
	{
		if (200 <= image->Get_UI_GroupID() && 250 >= image->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Image*>(image)->Set_OnOff(true);
		}
		if (250 == image->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Image*>(image)->Set_TexNumber(ItemInfo.ItemIconNum);
		}
	}


	return S_OK;
}

HRESULT CUIGroup_Iventory::DisConnect_MiniView_ItemInfo()
{
	_tchar* pTemp = {L""};

	dynamic_cast<CUIObject*>(m_pItemTextBoxName)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxDesc)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxCount)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxCountNum)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxType)->Set_Content(pTemp);
	for (auto& image : m_pItemScene->Find_UI_Image())
	{
		if (200 <= image->Get_UI_GroupID() && 250 >= image->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Image*>(image)->Set_OnOff(false);
		}

	}

	return S_OK;
}

CUIGroup_Iventory* CUIGroup_Iventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_Iventory* pInstance = new CUIGroup_Iventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_Iventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_Iventory::Clone(void* pArg)
{
	CUIGroup_Iventory* pInstance = new CUIGroup_Iventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_Iventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_Iventory::Free()
{
	__super::Free();
}

