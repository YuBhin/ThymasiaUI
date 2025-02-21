#pragma once

#include <process.h>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <iostream>

namespace Client 
{
	const unsigned int		g_iWinSizeX = 1600;
	const unsigned int		g_iWinSizeY = 900;

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };
	enum UISCENEID { UISCENE_MENU, UISCENE_LEVELUP, UISCENE_ATTRIBUTE, UISCENE_END };

	/* 컬링 거리 */
	const float g_fCullingDistance = 30.f; 
	

}

using namespace Client;

extern HINSTANCE g_hInst;
extern HWND g_hWnd;	


