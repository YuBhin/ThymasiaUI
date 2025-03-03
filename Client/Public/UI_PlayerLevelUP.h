#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Client)

class CUI_PlayerLevelUP final : public CUIObject
{
private:
	CUI_PlayerLevelUP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_PlayerLevelUP(const CUI_PlayerLevelUP& Prototype);
	virtual ~CUI_PlayerLevelUP() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName); // ���� UI �ҷ�����
	HRESULT LoadData_Text_Scene(); // ���� UI �ҷ�����
	//void Set_TexNumber(_int iNumber) { m_TexNumber = iNumber; }
private:
	//_int					m_TexNumber = {};
	vector<UI_TextInfo> m_TextInfo = {};

public:
	static CUI_PlayerLevelUP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END