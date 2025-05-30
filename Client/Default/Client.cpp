// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "pch.h" 
#include "Client.h"

#include "../Public/MainApp.h"
#include "GameInstance.h"
#include <stdio.h> // freopen, printf 등을 사용
#include <locale.h> // locale 적용
#include <tchar.h> // _tfreopen, _tprintf를 사용
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
HWND	g_hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

//HWND g_hIMGUIWIndow; // 임구이 핸들
//ImGuiContext* g_pIMGUIContext = nullptr; // 임구이 컨텍스트

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

//LRESULT CALLBACK IMGUIWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//HWND CreateIMGUIWindow(HINSTANCE hInstance);
//void RenderIMGUIWindow();

INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

/* 컴파일러가 스스로 작성한 코드(구동 코드)를 통해서 호출해주는 함수(진입점함수 or 메인함수). */
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow){

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.
	CMainApp*			pMainApp = { nullptr };


	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}


    pMainApp = CMainApp::Create();
    if (nullptr == pMainApp)
        return FALSE;

    //// 임구이 창 생성
    //g_hIMGUIWIndow = CreateIMGUIWindow(hInstance);
    //if (!g_hIMGUIWIndow)
    //{
    //    
    // 
    // (nullptr, L"Failed to Create IMGUI WIndow", L"Error", MB_OK);
    //    return false;
    //}


    //g_pIMGUIContext = ImGui::CreateContext();
    //ImGui::SetCurrentContext(g_pIMGUIContext);

    //pMainApp->SetUp_Window_IMGUI(g_hIMGUIWIndow);



	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	MSG msg;

	/* 싱글턴의 경우에는 할당한 객체의 주소를 스스로 자체에 보관(m_pInstance) 한다. */
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);	

	if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Default"))))
		return FALSE;
	if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_60"))))
		return FALSE;

	_float		fTimerAcc = {};

	// 기본 메시지 루프입니다.
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		pGameInstance->Compute_TimeDelta(TEXT("Timer_Default"));

		fTimerAcc += pGameInstance->Get_TimeDelta(TEXT("Timer_Default"));
		
		if (fTimerAcc >= 1.f / 120.f /*1초에 60번을 트루로 리턴*/)
		{
			pGameInstance->Compute_TimeDelta(TEXT("Timer_60"));

			pMainApp->Update(pGameInstance->Get_TimeDelta(TEXT("Timer_60")));

			pMainApp->Render();			

			fTimerAcc = 0.f;
		}		

      

	}

	Safe_Release(pGameInstance);

	if (0 != Safe_Release(pMainApp))
	{
		MSG_BOX("Failed to Release : CMainApp");
		return FALSE;
	}

	return (int) msg.wParam;
 }



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT		rcWindows = { 0, 0, g_iWinSizeX, g_iWinSizeY };

   AdjustWindowRect(&rcWindows, WS_OVERLAPPEDWINDOW, true);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, rcWindows.right - rcWindows.left, rcWindows.bottom - rcWindows.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   g_hWnd = hWnd;

   return TRUE;
}
//
//HWND CreateIMGUIWindow(HINSTANCE hInstance)
//{
//    WNDCLASS wc = {};
//    wc.lpfnWndProc = IMGUIWndProc;
//    wc.hInstance = hInstance;
//    wc.lpszClassName = L"IMGUIWindow";
//
//    RegisterClass(&wc);
//
//    HWND hWnd = CreateWindowEx(0, L"IMGUIWindow",L"WindowIMGUI",WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT, CW_USEDEFAULT,800,600, nullptr, nullptr, hInstance, nullptr);
//
//    if (!hWnd)
//    {
//        MessageBox(nullptr, L"Failed to Create IMGUI WIndow", L"Error", MB_OK);
//        return nullptr;
//    }
//
//    ShowWindow(hWnd, SW_SHOW);
//    UpdateWindow(hWnd);
//    return hWnd;
//
//}
//
//void RenderIMGUIWindow()
//{
//    ImGui::Render();
//    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//
//}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//LRESULT CALLBACK IMGUIWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
//    {
//        return true;
//    }
//
//    switch (message)
//    {
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//
//}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    {
        return true;
    }

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(hWnd, &ps);
        }
        break;


    case WM_DPICHANGED: // imgui 윈도우 포지션 재정의
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top
                , suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    case WM_CREATE:
        AllocConsole();
        SetConsoleTitle(TEXT("테스트용 콘솔"));
        /*AllocConsole 함수를 호출하여 콘솔창을 띄웠다면 freopen 함수로 기본 입출력 위치를 지정해야 합니다.*/
        FILE* fp;  // 파일 포인터 선언
        _wfreopen_s(&fp, L"CONOUT$", L"w", stdout);  // stdout 연결
        _wfreopen_s(&fp, L"CONERR$", L"w", stderr);  // stderr 연결
        _wfreopen_s(&fp, L"CONIN$", L"r", stdin);    // stdin 연결

        /* setlocale 함수로 기본 입출력에 대한 로케일을 설정합니다. */
        _tsetlocale(LC_ALL, _T(""));

        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
