/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <wx/event.h> 

namespace vapor { namespace editor {

//-----------------------------------//

/**
 * Input manager implementation using wxWidgets.
 * TODO: does nothing for now.
 */

class vaporInputManager : public vapor::input::InputManager
{
public:

	vaporInputManager();
	virtual ~vaporInputManager();

	/// Feeds a wxWidgets Keyboard input event to the input manager.
	void processKeyEvent( const wxKeyEvent& event, bool keyDown );

	/// Feeds a wxWidgets Mouse input event to the input manager.
	void processMouseEvent( const wxMouseEvent& event );

private:

	input::Keys::Enum convertKeyEnum( int keyCode );
};

//-----------------------------------//

} } // end namespaces