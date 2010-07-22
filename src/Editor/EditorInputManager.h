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

typedef fd::delegate<void(InputEvent&)> InputProcessCallback;

/**
 * Forwards all the input received by wxWidgets to the engine.
 */

class EditorInputManager : public InputManager
{
public:

	EditorInputManager();

	// Processes a wxWidgets keyboard input event.
	void processKeyEvent( const wxKeyEvent&, bool keyDown );

	// Processes a wxWidgets mouse input event.
	void processMouseEvent( const wxMouseEvent& );

	// Custom processing of input events.
	static void doKeyEvent( InputProcessCallback, const wxKeyEvent&, bool keyDown );
	static void doMouseEvent( InputProcessCallback, const wxMouseEvent& );

private:

	// Converts from wxWidgets events to vapor3D events.
	static Keys::Enum convertKeyEnum( int keyCode );

	InputProcessCallback cb;
};

//-----------------------------------//

} } // end namespaces