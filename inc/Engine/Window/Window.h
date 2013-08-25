/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Graphics/RenderTarget.h"
#include "Engine/Input/InputManager.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Represents an OS-specific native window handle.
 */
typedef void* WindowHandle;

/**
 * Represents window styles as bit-flags.
 */
enum class FLD_FLAGS WindowStyles
{
	None = 0,
	/// A top-level window is a window that is not a child.
	TopLevel   = 1 << 1,
	/// A miniframe is a window with minimal chrome.
	MiniFrame  = 1 << 2,
	/// A borderless window has no chrome.
	Borderless = 1 << 3,
	/// Fullscreen windows take the full space of the screen.
	Fullscreen = 1 << 4,
};

/** 
 * Represents window settings.
 */
class API_ENGINE FLD_VALUE_TYPE WindowSettings : public Settings
{
public:

	WindowSettings();
	WindowSettings(uint16 width, uint16 height, const String& title,
		WindowStyles styles);

	String title;
	WindowHandle handle;
	WindowStyles styles;
};

//-----------------------------------//

/**
 * Represents onscreen display areas in the target window system.
 */
class API_ENGINE Window : public RenderTarget
{
public:

	/// Creates a new window with the given settings.
	Window (const WindowSettings& settings);

	/// Creates a new render context.
	virtual RenderContext* createContext(const RenderContextSettings&) = 0;

	/// Updates the window content.
	void update() OVERRIDE {}

	/// Sets this rendering target as the current.
	void makeCurrent() OVERRIDE {}

	/// Shows/hides the window.
	virtual void show( bool visible = true ) {}

	/// Handles the event loop of the window.
	virtual bool pumpEvents() { return true; }

	/// Sets the title of the window.
	virtual void setTitle (const String& title) {}

	/// Sets the visibility of the mouse cursor.
	virtual void setCursorVisible( bool state ) {}

	/// Gets the visibility of the mouse cursor.
	virtual bool isCursorVisible() const { return false; }

	/// Sets the capture of the mouse cursor.
	virtual void setCursorCapture( bool state ) {}

	/// Gets the cursor position on screen.
	virtual Vector2i getCursorPosition() const { return Vector2i(0,0); }

	/// Sets the cursor position on screen.
	virtual void setCursorPosition( int x, int y );

	/// Gets if the window has focus.
	virtual bool hasFocus() { return false; }

	/// Gets the input manager.
	virtual InputManager* getInput() { return nullptr; }

	/// Gets the window settings.
	GETTER(Settings, const Settings&, settings)

	/// Event fired when the window is closed.
	Event0<> onWindowClose;

	/// Event fired when the window focus changes.
	Event1<bool> onWindowFocusChange;

protected:

	/// Handles the resize event.
	void handleWindowResize();

	/// Handles the close event.
	void handleWindowClose();   

	/// Handles the focus event.
	void handleWindowFocus( bool focusLost );

	/// Holds the window settings.
	WindowSettings settings;

	/// Render context associated with this window.
	RenderContextPtr renderContext;
};

//-----------------------------------//

NAMESPACE_ENGINE_END