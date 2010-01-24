/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
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

class wx_InputManager : public vapor::input::InputManager
{
public:

	wx_InputManager();
	virtual ~wx_InputManager();

	/// Feeds a wxWidgets Keyboard input event to the input manager.
	void processKeyEvent( const wxKeyEvent& event, bool keyDown );

	/// Feeds a wxWidgets Mouse input event to the input manager.
	void processMouseEvent( const wxMouseEvent& event );

private:

	input::Keys::Enum convertKeyEnum( int keyCode );
};

//-----------------------------------//

} } // end namespaces