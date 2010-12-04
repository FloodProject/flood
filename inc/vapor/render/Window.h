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

namespace vapor {

//-----------------------------------//

typedef void WindowHandle;

class VAPOR_API WindowSettings : public Settings
{
public:

	WindowSettings( const ushort width = 640, const ushort height = 480,
		const std::string& title = "Untitled", const bool fullscreen = false );

	std::string title;
	bool fullScreen;
	ushort bitsPerPixel;
	ushort depthBits;
	ushort stencilBits;
	ushort antialiasLevel;
	WindowHandle* handle;
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
	virtual void update() {} // = 0;

	// Sets this rendering target as the current.
	virtual void makeCurrent() {} // = 0;

	// Shows/hides the window.
	virtual void show( bool hide = false ) {} // = 0;

	// Handles the event loop of the window.
	virtual bool pumpEvents() { return true; } // = 0;

	// Sets the title of the window.
	virtual void setTitle (const std::string& title) {} // = 0;

	// Gets the input manager.
	virtual InputManager* getInputManager() { return nullptr; } // = 0;

	// Sets the visibility of the mouse cursor.
	virtual void setCursorVisible( bool state ) {} // = 0;

	// Gets the visibility of the mouse cursor.
	virtual bool isCursorVisible() const { return false; } // = 0;

	// Gets the cursor position on screen.
	virtual Vector2i getCursorPosition() const { return Vector2i::Zero; } // = 0;

	// Sets the cursor position on screen.
	virtual void setCursorPosition( int x, int y ) {} // = 0;
	void setCursorPosition( const Vector2i& pos );

	// Gets the window settings.
	GETTER(Settings, const Settings&, settings)

	// Event fired when the window is closed.
	Event0<> onWindowClose;

	// Event fired when the window focus changes.
	Event1<bool> onWindowFocusChange;

	// Create a new render window.
	static Window* createWindow( const WindowSettings& = WindowSettings() );

protected:

	// Handles the resize event.
	void handleWindowResize();

	// Handles the resize event.
	void handleWindowClose();

	// Handles the focus event.
	void handleWindowFocus( bool focusLost );

	// Holds the window settings.
	WindowSettings settings;
};

//-----------------------------------//

} // end namespace