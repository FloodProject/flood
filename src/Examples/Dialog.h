/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <vapor/Engine.h>

#include "resource.h"

using namespace vapor::resources;
using namespace vapor::render;

//-----------------------------------//

BOOL CALLBACK DialogProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		// sets the window title
		SetWindowText(hWnd, TEXT("vaporEngine Framework"));
		return TRUE;

	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------//

class Dialog
{

public:

	Dialog()
	{
		HINSTANCE hInstance = GetModuleHandle(nullptr);

		HWND hDialog = CreateDialog (hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);

		ShowWindow(hDialog, SW_SHOW); 

		MSG msg;

		while(GetMessage(&msg, NULL, 0, 0) > 0)
		{
			if(!IsDialogMessage(hDialog, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
};

//-----------------------------------//