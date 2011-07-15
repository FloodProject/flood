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
#include "Network/Network.h"

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

void NetworkingPlugin::connect()
{
	client = Allocate(NetworkClient, AllocatorGetHeap());

	String address("127.0.0.1");

	if( !client->connect(address, 9999) )
		LogError("Error connecting to server at '%s'", address.c_str());
	else
		LogInfo("Connected to server at '%s'", address.c_str());

	client->checkEvents(5000);
}

//-----------------------------------//

NAMESPACE_EDITOR_END