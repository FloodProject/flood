/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "MonoPlugin.h"
#include "Editor.h"
#include "Core/Utilities.h"

#ifdef PLUGIN_MONO

#include <mono/metadata/mono-config.h>
#include <mono/mini/jit.h>
#include <mono/metadata/assembly.h>

MonoDomain* domain;

namespace vapor { namespace editor {

//-----------------------------------//

REFLECT_CHILD_CLASS(MonoPlugin, EditorPlugin)
REFLECT_CLASS_END()

static MonoString* callback()
{
	return mono_string_new(mono_domain_get(), "Callback test!");
}

static void Mono_Log(MonoString* string)
{
	LogInfo( mono_string_to_utf8(string) );
}

//-----------------------------------//

MonoPlugin::MonoPlugin()
	: domain(nullptr)
{ }

//-----------------------------------//

PluginMetadata MonoPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Mono";
	metadata.description = "Provides .NET scripting via Mono";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.startEnabled = false;

	return metadata;
}

//-----------------------------------//

void MonoPlugin::onPluginEnable()
{
	std::string dir = PathGetCurrentDir()
		+ PathGetSeparator()
		+ "Plugins";

	mono_set_dirs(dir.c_str(), dir.c_str());

	// Load the default Mono configuration file.
	mono_config_parse(nullptr);
	
	// Creates a new domain where each assembly is loaded and run.
	domain = mono_jit_init_version("vapor3D Root Domain", "v2.0.50727");

	// Creates and sets a new child domain.
	domainEditor = mono_domain_create_appdomain("vapor3D Child Domain", nullptr);
	
	// Set new domain as the default domain.
	mono_domain_set(domainEditor, 0);
	
	mono_add_internal_call("Editor::GetString", callback);
	mono_add_internal_call("Editor::Log", Mono_Log);

	MonoAssembly* assembly = nullptr;
	assembly = mono_domain_assembly_open(domainEditor, "Plugins/EditorSupport.exe");

	char* args[] = { "Foo", "Test Argument #1", nullptr };
	mono_jit_exec(domainEditor, assembly, 2, args);
}

//-----------------------------------//

void MonoPlugin::onPluginDisable()
{
	mono_jit_cleanup(domain);
	mono_config_cleanup();

	mono_domain_set(domain, 0);

	// Clean up the domain.
	mono_domain_unload(domainEditor);

	domain = nullptr;
}

//-----------------------------------//

} } // end namespaces

#endif