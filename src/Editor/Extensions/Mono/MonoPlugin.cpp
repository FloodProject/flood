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

#ifdef ENABLE_PLUGIN_MONO

#include "../interface/Bindings/Engine_wrap.cxx"

#include <mono/metadata/mono-config.h>
#include <mono/mini/jit.h>
#include <mono/metadata/assembly.h>

MonoDomain* domain;

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(MonoScript, Script)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(MonoPlugin, EditorPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

MonoPlugin::MonoPlugin()
	: domain(nullptr)
{
}

//-----------------------------------//

PluginMetadata MonoPlugin::getMetadata()
{
	static PluginMetadata metadata;
	
	metadata.name = "Mono";
	metadata.description = "Provides .NET scripting via Mono";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.startEnabled = true;

	return metadata;
}

//-----------------------------------//

#ifdef PLATFORM_WINDOWS
static INLINE LPTOP_LEVEL_EXCEPTION_FILTER GetUnhandledExceptionFilter(void)
{
	/* Windows does not provide a GetUnhandledExceptionFilter()
	* function, so simulate it by unsetting the handler (getting the
	* previous handler) and then re-setting the handler.
	*/
	LPTOP_LEVEL_EXCEPTION_FILTER handler = SetUnhandledExceptionFilter(NULL);
	SetUnhandledExceptionFilter(handler);
	return handler;
}

static LPTOP_LEVEL_EXCEPTION_FILTER gs_MonoUnhandledFilter = nullptr; 
#endif

void MonoPlugin::onPluginRegistered()
{
	// When Mono is loaded as a DLL, it needs to have the threading initialized early.
	
	Path dir = PathCombine(PathGetCurrentDir(), "Plugins");
	mono_set_dirs(dir.c_str(), dir.c_str());

	// Load the default Mono configuration file.
	mono_config_parse(nullptr);

	// Creates a new domain where each assembly is loaded and run.
	domain = mono_jit_init_version("Root Domain", "v2.0.50727");

	const char* error = mono_check_corlib_version();
	
	if(error)
	{
		LogError("Corlib not in sync with this runtime: %s", error);
		return;
	}

#ifdef PLATFORM_WINDOWS
	gs_MonoUnhandledFilter = GetUnhandledExceptionFilter();
#endif
}

//-----------------------------------//

void MonoPlugin::onPluginEnable()
{
	// Creates and sets a new child domain.
	domainEditor = mono_domain_create_appdomain("Child Domain", nullptr);
	
	// Set new domain as the default domain.
	mono_domain_set(domainEditor, false);
	
	mainAssembly = nullptr;
	mainMethod = nullptr;
	updateMethod = nullptr;

	initialize();
}

//-----------------------------------//

void MonoPlugin::initialize()
{
	mainAssembly = mono_domain_assembly_open(domainEditor, "EditorManaged.dll");

	if( !mainAssembly )
	{
		LogError("The managed implementation of the editor was not found");
		return;
	}

	MonoImage* image = mono_assembly_get_image(mainAssembly);
	assert( image != nullptr );

	MonoClass* klass = mono_class_from_name(image, "Flush", "Main");
	
	if( !klass )
	{
		LogError("Main class was not found");
		return;
	}

	mainMethod = mono_class_get_method_from_name(klass, "Init", 0);

	if( !mainMethod )
	{
		LogError("Init() method was not found");
		return;
	}

	call(mainMethod);

	updateMethod = mono_class_get_method_from_name(klass, "Update", 0);

	if( !updateMethod )
	{
		LogError("Update() method was not found");
		return;
	}

	call(updateMethod);
}

//-----------------------------------//

void MonoPlugin::call(MonoMethod* method)
{
	if( !method ) return;

	MonoObject* exception = nullptr;

#ifdef PLATFORM_WINDOWS
	/* Workaround for SEH exception handling. The top-level exception filter
	 * set by SetUnhandledExceptionFilter isn't called when in debug mode.
	 * Behaviour documented here: http://support.microsoft.com/kb/173652/en-us
	 * We add our own SEH try-catch block and redirect to the Mono defined handler.
	 */
	__try
	{
		mono_runtime_invoke(method, nullptr, nullptr, &exception);
	}
	__except ( gs_MonoUnhandledFilter( GetExceptionInformation() ) )
	{
	}
#endif

	if( exception != nullptr )
	{
		MonoString* trace = mono_object_to_string(exception, nullptr);
		assert( trace != nullptr );

		LogError("Exception from managed code:\n%s", mono_string_to_utf8(trace));
		return;
	}
}

//-----------------------------------//

void MonoPlugin::onPluginUpdate()
{
	call(updateMethod);
}

//-----------------------------------//

void MonoPlugin::onPluginDisable()
{
	// Set the initial domain.
	mono_domain_set(domain, false);

	// Clean up the editor domain.
	mono_domain_unload(domainEditor);

	// Cleanup the Mono JIT context.
	mono_jit_cleanup(domain);

	//mono_config_cleanup();
}

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif