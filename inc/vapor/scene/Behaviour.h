/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Component.h"

FWD_DECL_INTRUSIVE(Script)

namespace vapor {

//-----------------------------------//

class State;

//-----------------------------------//

/**
 * Script that can be executed by a script state.
 */

class VAPOR_API Behaviour : public Component
{
	DECLARE_CLASS_()

public:

	// Invokes a function on the behaviour.
	bool invoke( const std::string& name );

	// Updates the script.
	virtual void update( double delta );

protected:

	// Script name.
	std::string scriptName;

	// Source code of the script.
	ScriptPtr script;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Script );

//-----------------------------------//

} // end namespace