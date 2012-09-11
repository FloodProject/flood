/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Engine/Subsystem.h"
#include "Script/ScriptState.h"
#include "Resources/ResourceManager.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Manages all the script instances in the engine.
 */

class API_ENGINE ScriptManager : public Subsystem
{
public:

	ScriptManager();
	~ScriptManager();

	// Creates a new script instance.
	ScriptState* createScriptInstance(Script*);

	// Updates all the registered scripts.
	void update() OVERRIDE;

protected:

	// Gets called when a resource is reloaded.
	void onReload( const ResourceEvent& event );
};

//-----------------------------------//

NAMESPACE_ENGINE_END