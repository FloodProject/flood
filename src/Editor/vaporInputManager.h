/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

//-----------------------------------//

typedef fd::delegate<void(Event&)> InputProcessCallback;

/**
 * Input manager implementation using wxWidgets.
 */

class vaporInputManager : public InputManager
{
public:

	vaporInputManager();

	/// Feeds a wxWidgets Keyboard input event to the input manager.
	void processKeyEvent( const wxKeyEvent&, bool keyDown );

	/// Feeds a wxWidgets Mouse input event to the input manager.
	void processMouseEvent( const wxMouseEvent& );

	/// Converts from wxWidgets events to vapor events.
	static void doKeyEvent( InputProcessCallback, const wxKeyEvent&, bool keyDown );
	static void doMouseEvent( InputProcessCallback, const wxMouseEvent& );

private:

	static Keys::Enum convertKeyEnum( int keyCode );
	InputProcessCallback cb;
};



//-----------------------------------//

} } // end namespaces