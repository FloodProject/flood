/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#include "Script/ScriptManager.h"
#include "Resources/ResourceManager.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

ScriptManager::ScriptManager()
{
	ResourceManager* res = GetResourceManager();
	res->onResourceReloaded.Connect( this, &ScriptManager::onReload );
}

//-----------------------------------//

ScriptManager::~ScriptManager()
{
	ResourceManager* res = GetResourceManager();
	res->onResourceReloaded.Disconnect( this, &ScriptManager::onReload );
}

//-----------------------------------//

void ScriptManager::onReload( const ResourceEvent& event )
{
#if 0
	const ScriptHandle& handle = HandleCast<Script>(event.handle);
	Script* script = handle.Resolve();

	if( script->getResourceGroup() != ResourceGroup::Scripts )
		return;
	
	LogDebug( "Reloading script '%s'", script->getPath().c_str() );

	ScriptsMap::const_iterator it;
	for( it = scripts.begin(); it != scripts.end(); it++ )
	{
		if( script != it->second )
			continue;

		State* state = it->first;
		assert( state != nullptr );
		
		state->load( script );
	}
#endif
}

//-----------------------------------//

void ScriptManager::update()
{

}

//-----------------------------------//

NAMESPACE_ENGINE_END