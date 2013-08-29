/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Script/ScriptState.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

ScriptState::ScriptState()
{
}

//-----------------------------------//

ScriptState::~ScriptState()
{
}

//-----------------------------------//

bool ScriptState::load(Script* script)
{
	if( !script ) return false;

	return true;
}

//-----------------------------------//

bool ScriptState::execute( Script* script )
{
	return execute( script->getSource() );
}

//-----------------------------------//

bool ScriptState::execute( const String& source )
{
	return true;
}

//-----------------------------------//

bool ScriptState::invoke( const String& name, uint8 numArguments )
{
	return true;
}

//-----------------------------------//

void ScriptState::handleError()
{
}

//-----------------------------------//

NAMESPACE_ENGINE_END