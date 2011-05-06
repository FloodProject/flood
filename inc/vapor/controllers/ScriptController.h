/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Controllers/Controller.h"
#include "Script/Script.h"

struct swig_module_info;

namespace vapor {

//-----------------------------------//

class State;
struct KeyEvent;
struct MouseButtonEvent;

/**
 * Script that can be executed by a script state.
 */

REFLECT_DECLARE_CLASS(ScriptController)

class VAPOR_API ScriptController : public Controller
{
	REFLECT_DECLARE_OBJECT(ScriptController)

public:

	ScriptController();
	~ScriptController();

	// Updates the script.
	virtual void _update( float delta );

protected:

	// Creates a new state.
	void createState();

	// Binds the node instance.
	void bindEntity(Entity* entity);

	// Binds a type into this instance.
	void bindType(swig_module_info* module, const char* name, const char* type, void* object);

	// Gets the script by its name.
	ScriptHandle getScript();

	// Input callback functions.
	virtual void onKeyPress( const KeyEvent& );
	virtual void onKeyRelease( const KeyEvent& );
	virtual void onMouseButtonPressed( const MouseButtonEvent& );
	virtual void onMouseButtonReleased( const MouseButtonEvent& );

	// Source code of the script.
	ScriptHandle script;

	// Scripting state.
	State* state;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Script );

//-----------------------------------//

} // end namespace