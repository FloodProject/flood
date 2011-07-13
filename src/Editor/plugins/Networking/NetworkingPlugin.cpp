/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "NetworkingPlugin.h"
#include "Editor.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CLASS(NetworkingPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

NetworkingPlugin::NetworkingPlugin()
{

}

//-----------------------------------//

PluginMetadata NetworkingPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Networking";
	metadata.description = "Handles connecting to remote hosts.";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void NetworkingPlugin::onPluginEnable()
{
	PluginMetadata metadata = getMetadata();

	LogDebug( "Plugin '%s': Networking", metadata.name.c_str() );
}

//-----------------------------------//

void NetworkingPlugin::onPluginDisable()
{
	PluginMetadata metadata = getMetadata();

	LogDebug( "Plugin '%s': Bye", metadata.name.c_str() );
}

//-----------------------------------//

NAMESPACE_EDITOR_END