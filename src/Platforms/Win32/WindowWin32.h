/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_WINDOW_WIN32

#include "Window/Window.h"

#ifdef PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#undef  NOMINMAX
	#define NOMINMAX
	#include <Windows.h>	
#endif

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Window implementation that uses native Windows API calls.
 */

class Win32Window : public Window
{
	friend LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:

	Win32Window(const WindowSettings& settings);
	virtual ~Win32Window();

	// Gets the window handle.
	WindowHandle* getHandle();

	// Swaps the buffers (updates the display).
	void update();

	// Handle the window message events.
	bool pumpEvents();

	// Sets the title of the window.
	bool setTitle(const std::wstring& title);

	// Sets the cursor visibility.
	void setCursor(bool state);

	// Sets this context as the current.
	void makeCurrent();

protected:

	// Registers the Win32 window class.
	bool registerClass();

	// Creates the window.
	bool createWindow();

	bool createContext();

	// Get a string of the latest Windows error.
	std::string getErrorMessage();

	// Instance handle.
	HINSTANCE hInstance;

	// Device context.
	HDC hDC;

	// Window handle.
	HWND hWnd;

	// GL rendering context.
	HGLRC hRC;

	// Windows procedure.
	WNDPROC windowProcedure;
};

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif