/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector2.h"
#include "vapor/render/Target.h"
#include "vapor/input/InputManager.h"

FWD_DECL_TYPEDEF_PTR(Window)

namespace vapor {

//-----------------------------------//

/**
 * Represents the information that the user wants to pass to the engine
 * related to the creation of a new rendering window, like for example
 * bits-per-pixel, the size of the the window, if it is fullscreen, and 
 * in the future, anti-aliasing settings, and custom window handle hooks.
 */

class VAPOR_API WindowSettings : public Settings
{
public:

	WindowSettings( const ushort width = 640, const ushort height = 480,
		const std::string& title = "Untitled", const bool fullscreen = false );

	WindowSettings( const WindowSettings& settings );

	// Gets/sets the window title.
	ACESSOR(Title, const std::string&, title)

	// Gets/sets the bits-per-pixel of the window.
	ACESSOR(Bpp, const ushort, bitsPerPixel)

	// Gets/sets the size of the window's depth buffer.
	ACESSOR(DepthBits, const ushort, depthBits)

	// Gets/sets the size of the window's stencil buffer.
	ACESSOR(StencilBits, const ushort, stencilBits)

	// Gets/sets the antialiasing level of the window.
	ACESSOR(Antialiasing, const ushort, antialiasLevel)

	// Gets/sets the custom handle of the window.
	ACESSOR(CustomHandle, void*, customHandle)

	// Is this window fullscreen?
	const bool isFullscreen() const { return fullScreen; }

protected:

	std::string title;
	bool fullScreen;
	ushort bitsPerPixel;
	ushort depthBits;
	ushort stencilBits;
	ushort antialiasLevel;
	void* customHandle;
};

//-----------------------------------//

/**
 * Represents an region on the screen that the renderer can send
 * the final representation of the scene, the rendered output image.
 * In most platforms (PCs) this will be just a normal window on the
 * desktop, but on some platforms (consoles, for instance) this might 
 * be slighty different, so some methods might not make much sense. 
 */

class VAPOR_API Window : public RenderTarget
{
public:

	Window (const WindowSettings& settings);

	// Updates the window content.
	virtual void update() = 0;

	// Sets this rendering target as the current.
	virtual void makeCurrent() = 0;

	// Shows/hides the window.
	virtual void show( bool hide = false ) = 0;

	// Handles the event loop of the window.
	virtual bool pumpEvents() = 0;

	// Sets the title of the window.
	virtual void setTitle (const std::string& title) = 0;

	// Sets the visibility of the mouse cursor.
	virtual void setCursorVisible( bool state ) = 0;

	// Gets the visibility of the mouse cursor.
	virtual bool isCursorVisible() const = 0;

	// Gets the cursor position on screen.
	virtual Vector2i getCursorPosition() const = 0;

	// Sets the cursor position on screen.
	virtual void setCursorPosition( int x, int y ) = 0;
	void setCursorPosition( const Vector2i& pos );

	// Gets the window settings.
	GETTER(Settings, const Settings&, settings)

	// Gets the input manager.
	virtual InputManager& getInputManager() = 0;

	// Event fired when the window is closed.
	fd::delegate< void() > onWindowClose;

	// Event fired when the window's focus changes.
	fd::delegate< void( bool focusLost ) > onWindowFocusChange;

	// Create a new render window.
	static WindowPtr createWindow( const WindowSettings& = WindowSettings() );

protected:

	// Handles the resize event.
	void handleWindowResize();

	// Handles the resize event.
	void handleWindowClose();

	// Handles the focus event.
	void handleWindowFocus( bool focusLost );

	// Holds the window settings
	WindowSettings settings;
};

TYPEDEF_PTR(Window)

//-----------------------------------//

} // end namespace