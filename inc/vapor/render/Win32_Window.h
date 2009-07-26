/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_WINDOWING_WIN32

#include "vapor/Platform.h"
#include "vapor/render/Window.h"

#ifndef VAPOR_PLATFORM_WINDOWS
	#error This code is Windows only. Disable VAPOR_WINDOWING_WIN32.
#endif

namespace vapor {
	namespace render {

class Win32Window : public Window
{
public:

	Win32Window(Settings& settings);
	virtual ~Win32Window();

	// Swaps the buffers (updates the display)
	void update();

	// Handle the window message events
	bool pumpEvents();

	// Sets the title of the window
	void setTitle(const std::string& title) const;

	// Sets the cursor visibility
	void setCursor(bool state) const;

	void makeCurrent();

private:

	// Opens a new window
	bool create();

	// Device context
	HDC hDC;

	// Window handle
	HWND hWnd;
};

} } // end namespaces

#endif