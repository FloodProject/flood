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
#include "vapor/resources/ResourceManager.h"
#include "vapor/Engine.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Behaviour, Component)
	FIELD_PRIMITIVE(Behaviour, string, scriptName)
	FIELD_CLASS_PTR(Behaviour, Script, script)
END_CLASS()

//-----------------------------------//

bool Behaviour::invoke( const std::string& name )
{
	return true;
}

//-----------------------------------//

void Behaviour::update( double delta )
{
	Engine* engine = Engine::getInstancePtr();

	if( !script && !scriptName.empty() )
	{
		ResourceManager* resources = engine->getResourceManager();
		script = resources->loadResource<Script>(scriptName);
	}

	if( !script )
		return;

	//script->execute();
}

//-----------------------------------//

} // end namespace