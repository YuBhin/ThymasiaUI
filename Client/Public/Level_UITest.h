#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_UITest final : public CLevel
{
private:
	CLevel_UITest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_UITest() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	// UI ¾À
	HRESULT Ready_Layer_UIGroup_GameIntro(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerMenu(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerLevelUP(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_Iventory(const _tchar* pLayerTag);

private:
	_bool m_bUIOpen = {false};


public:
	static CLevel_UITest* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END