/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/input/InputManager.h"

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

private:

};

//-----------------------------------//