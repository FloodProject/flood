/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "SamplePlugin.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

SamplePlugin::SamplePlugin( EditorFrame* frame )
	: Plugin(frame)
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

	debug( "Plugin '%s': Hello", metadata.name.c_str() );
}

//-----------------------------------//

void SamplePlugin::onPluginDisable()
{
	PluginMetadata metadata = getMetadata();

	debug( "Plugin '%s': Bye", metadata.name.c_str() );
}

//-----------------------------------//

} } // end namespaces