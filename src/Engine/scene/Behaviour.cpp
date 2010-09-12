/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Behaviour.h"
#include "vapor/script/Script.h"
#include "vapor/script/ScriptManager.h"
#include "vapor/resources/ResourceManager.h"
#include "vapor/Engine.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Behaviour, Controller)
	FIELD_PRIMITIVE(Behaviour, string, scriptName)
	FIELD_CLASS_PTR(Behaviour, Script, script)
END_CLASS()

//-----------------------------------//

Behaviour::Behaviour()
	: state(nullptr)
{ }

//-----------------------------------//

void Behaviour::_update( double delta )
{
	if( !script && !scriptName.empty() )
		script = getScript();

	if( !state )
		state = createState();

	if( state )
		state->invoke("onUpdate");
}

//-----------------------------------//

State* Behaviour::createState()
{
	Engine* engine = Engine::getInstancePtr();

	if( !script )
		return nullptr;
	
	ScriptManager* scripts = engine->getScriptManager();
	return scripts->createScriptInstance(script);
}

//-----------------------------------//

ScriptPtr Behaviour::getScript()
{
	Engine* engine = Engine::getInstancePtr();

	ResourceManager* resources = engine->getResourceManager();
	return resources->loadResource<Script>(scriptName);
}

//-----------------------------------//

} // end namespace