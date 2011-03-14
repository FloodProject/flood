/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "SamplePlugin.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

REFLECT_ABSTRACT_CLASS(SamplePlugin)
REFLECT_END()

//-----------------------------------//

SamplePlugin::SamplePlugin()
{ }

//-----------------------------------//

PluginMetadata SamplePlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Sample";
	metadata.description = "Logs a message at plugin enable and disable";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void SamplePlugin::onPluginEnable()
{
	PluginMetadata metadata = getMetadata();

	LogDebug( "Plugin '%s': Hello", metadata.name.c_str() );
}

//-----------------------------------//

void SamplePlugin::onPluginDisable()
{
	PluginMetadata metadata = getMetadata();

	LogDebug( "Plugin '%s': Bye", metadata.name.c_str() );
}

//-----------------------------------//

} } // end namespaces