/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Target.h"
#include "vapor/input/InputManager.h"
#include "vapor/math/Vector2.h"

namespace vapor { namespace render {

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
		const bool fullscreen = false, const std::string title = "Untitled",
		const ushort stencilbits = 8, const ushort bpp = 32, 
		const ushort depthbits = 24, const ushort aalevel = 0, 
		void* customHandle = nullptr );

	// Gets/sets the window title.
	IMPLEMENT_ACESSOR(Title, const std::string&, title)

	// Gets/sets the custom handle of the window.
	IMPLEMENT_ACESSOR(CustomHandle, void*, customHandle)

	// Gets/sets the bits-per-pixel of the window.
	IMPLEMENT_ACESSOR(Bpp, const ushort, bpp)

	// Gets/sets the size of the window's depth buffer.
	IMPLEMENT_ACESSOR(DepthBits, const ushort, depthbits)

	// Gets/sets the size of the window's stencil buffer.
	IMPLEMENT_ACESSOR(StencilBits, const ushort, stencilbits)

	// Gets/sets the antialiasing level of the window.
	IMPLEMENT_ACESSOR(AntiAliasing, const ushort, aalevel)

	// Is this window fullscreen?
	const bool isFullscreen() const { return fullscreen; }

protected:

	std::string title;
	bool fullscreen;

	ushort bpp;
	ushort depthbits;
	ushort stencilbits;
	ushort aalevel;
	
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
	virtual ~Window ();

	// Updates the window content.
	virtual void update() = 0;

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
	virtual math::Vector2i getCursorPosition() const = 0;

	// Sets the cursor position on screen.
	virtual void setCursorPosition( int x, int y ) = 0;

	// Gets the window settings.
	virtual const Settings& getSettings();

	// Gets the input manager.
	virtual input::InputManager& getInputManager() = 0;

	// Create a new render window.
	static Window& createWindow( 
		const WindowSettings& settings = WindowSettings() );

	// Event fired when the window is closed.
	fd::delegate< void() > onWindowClose;

	// Event fired when the window's focus changes.
	fd::delegate< void( bool focusLost ) > onWindowFocusChange;

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

//-----------------------------------//

} } // end namespaces