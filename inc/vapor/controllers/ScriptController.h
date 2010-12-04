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

struct swig_module_info;

namespace vapor {

//-----------------------------------//

class State;
struct KeyEvent;
struct MouseButtonEvent;

//-----------------------------------//

/**
 * Script that can be executed by a script state.
 */

class VAPOR_API ScriptController : public Controller
{
	DECLARE_CLASS_()

public:

	ScriptController();
	~ScriptController();

	// Updates the script.
	virtual void _update( double delta );

protected:

	// Creates a new state.
	void createState();

	// Binds the node instance.
	void bindEntity();

	// Binds a type into this instance.
	void bindType(swig_module_info* module, const char* name,
		const char* type, void* object);

	// Gets the script by its name.
	ScriptPtr getScript();

	// Input callback functions.
	virtual void onKeyPress( const KeyEvent& );
	virtual void onKeyRelease( const KeyEvent& );
	virtual void onMouseButtonPressed( const MouseButtonEvent& );
	virtual void onMouseButtonReleased( const MouseButtonEvent& );

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