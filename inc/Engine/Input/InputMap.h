/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Input/InputManager.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/Joystick.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Input actions abstract an action triggered by an input binding.
 * This allows the code that handles the action to be unaware of the
 * specific combination that triggered the action, and provides
 * support for remapping of actions by the users of the application.
 */

class API_INPUT InputAction
{
public:

	// Adds a new binding for the action.
	void addBinding();

	// Name of the action.
	String name;
};

//-----------------------------------//

/**
 * Input schemas provide grouping for input actions. This can be useful
 * when you have distinct modes in your application, for example a 'Menu'
 * schema, a 'Driving' schema aswell as a 'Flying' schema.
 */

class API_INPUT InputSchema
{
public:

	// Creates a new input action.
	InputAction* createAction(const String& name);

	// Name of the schema.
	String name;
};

//-----------------------------------//

/**
 * Manages all the input schemas, bindings and keeps track of
 * the current schema aswell as matching the input events to
 * the specific input actions in the schemas.
 */

class API_INPUT InputMap
{
	DECLARE_UNCOPYABLE(InputMap);

public:

	InputMap( const InputManager& input );
	~InputMap();

	// Creates a new input schema.
	InputSchema* createSchema(const String& name);

	// Sets the current input schema.
	void setSchema(InputSchema* schema);

protected:

	// Input callbacks.
	void onKeyPress(const KeyEvent& event);
	void onMousePress(const MouseButtonEvent& event);
	void onJoyPress(const JoyButtonEvent& event);
};

//-----------------------------------//

NAMESPACE_ENGINE_END
