/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Input/InputManager.h"
#include "Engine/Input/Keyboard.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

/**
 * Forwards all the input received by wxWidgets to the engine.
 */

class EditorInputManager
{
public:

	EditorInputManager(InputManager* input);

	// Processes a wxWidgets keyboard input event.
	void processKeyEvent( const wxKeyEvent&, bool keyDown );

	// Processes a wxWidgets char keyboard input event.
	void processCharEvent( const wxKeyEvent& );

	// Processes a wxWidgets mouse input event.
	void processMouseEvent( const wxMouseEvent& );

	InputManager* input;
};

//-----------------------------------//

NAMESPACE_EDITOR_END