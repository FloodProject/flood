/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/controllers/Controller.h"

FWD_DECL_INTRUSIVE(Script)

namespace vapor {

//-----------------------------------//

class State;

//-----------------------------------//

/**
 * Script that can be executed by a script state.
 */

class VAPOR_API Behaviour : public Controller
{
	DECLARE_CLASS_()

public:

	Behaviour();

	// Invokes a function on the behaviour.
	bool invoke( const std::string& name );

	// Updates the script.
	virtual void _update( double delta );

protected:

	// Creates a new state.
	State* createState();

	// Gets the script by its name.
	ScriptPtr getScript();

	// Script name.
	std::string scriptName;

	// Source code of the script.
	ScriptPtr script;

	// Scripting state.
	State* state;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Script );

//-----------------------------------//

} // end namespace