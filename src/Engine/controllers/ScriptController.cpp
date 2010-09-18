/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/controllers/ScriptController.h"
#include "vapor/script/Script.h"
#include "vapor/script/ScriptManager.h"
#include "vapor/resources/ResourceManager.h"
#include "vapor/Engine.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(ScriptController, Controller)
	FIELD_PRIMITIVE(ScriptController, string, scriptName)
	FIELD_CLASS_PTR(ScriptController, Script, script)
END_CLASS()

//-----------------------------------//

ScriptController::ScriptController()
	: state(nullptr)
{ }

//-----------------------------------//

void ScriptController::_update( double delta )
{
	if( !script && !scriptName.empty() )
		script = getScript();

	if( !state )
		state = createState();

	if( state )
		state->invoke("onUpdate");
}

//-----------------------------------//

State* ScriptController::createState()
{
	Engine* engine = Engine::getInstancePtr();

	if( !script )
		return nullptr;
	
	ScriptManager* scripts = engine->getScriptManager();
	return scripts->createScriptInstance(script);
}

//-----------------------------------//

ScriptPtr ScriptController::getScript()
{
	Engine* engine = Engine::getInstancePtr();

	ResourceManager* resources = engine->getResourceManager();
	return resources->loadResource<Script>(scriptName);
}

//-----------------------------------//

} // end namespace