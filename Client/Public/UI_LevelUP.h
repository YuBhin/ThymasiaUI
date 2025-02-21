#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Client)

class CUI_LevelUP final : public CUIObject
{
private:
	CUI_LevelUP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_LevelUP(const CUI_LevelUP& Prototype);
	virtual ~CUI_LevelUP() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName); // 현재 UI 불러오기
	//void Set_TexNumber(_int iNumber) { m_TexNumber = iNumber; }

private:
	//_int					m_TexNumber = {};

public:
	static CUI_LevelUP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END