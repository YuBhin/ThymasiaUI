#pragma once


#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

/* 절차지향(Window API) 만으로 게임을 구현하기 힘들기 때문에 */
/* 내 코드의 흐름을 CMainApp클래스의 반복적인 업데이트와 렌더쪽을 당겨온다. */
BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

private: /* 엔진의 기능을 이용ㅇ하기위해 게임인스튼서 클래스르 ㄱㄹ객체화 해놓았다. */
	CGameInstance*			m_pGameInstance = { nullptr };
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

#ifdef _DEBUG
private:
	_float				m_fTimerAcc = {};
	_uint				m_iDrawCount = {};
	_tchar				m_szFPS[MAX_PATH] = {};
#endif

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

public:
	HRESULT Start_Level(LEVELID eStartLevelID);

	//imgui 세팅 함수
	HRESULT SetUp_ImGUI(ID3D11Device* _ppDevice, ID3D11DeviceContext* _ppContext);
	HRESULT SetUp_Window_IMGUI(HWND hWnd);
	HRESULT Render_Window_IMGUI(HWND hWnd);
	//imgui 폰트 설정
	void	LoadFont_ImGUI();

	// 티메시아 폰트
	HRESULT	LoadFont_Thymasia();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END