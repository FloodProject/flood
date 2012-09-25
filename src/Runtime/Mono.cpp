/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Runtime/API.h"
#include "Runtime/Mono.h"
#include "Core/Utilities.h"

#include <mono/metadata/mono-config.h>
#include <mono/mini/jit.h>
#include <mono/metadata/assembly.h>
#include <eglib/src/glib.h>

#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

static LPTOP_LEVEL_EXCEPTION_FILTER GetUnhandledExceptionFilter(void)
{
	/* Windows does not provide a GetUnhandledExceptionFilter()
	* function, so simulate it by unsetting the handler (getting the
	* previous handler) and then re-setting the handler.
	*/
	LPTOP_LEVEL_EXCEPTION_FILTER handler = SetUnhandledExceptionFilter(NULL);
	SetUnhandledExceptionFilter(handler);
	return handler;
}

static LPTOP_LEVEL_EXCEPTION_FILTER gs_MonoUnhandledExceptionFilter = nullptr; 

#endif

NAMESPACE_RUNTIME_BEGIN

//-----------------------------------//

//REFLECT_CHILD_CLASS(MonoScript, Script)
//REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(MonoRuntime, Plugin)
REFLECT_CLASS_END()

//-----------------------------------//

MonoRuntime::MonoRuntime()
	: domain(nullptr)
{
}

//-----------------------------------//

PluginMetadata MonoRuntime::getMetadata()
{
	static PluginMetadata metadata;
	
	metadata.name = "Mono";
	metadata.description = "Provides .NET scripting via Mono";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.startEnabled = false;

	return metadata;
}

//-----------------------------------//

void MonoRuntime::onPluginEnable()
{
	initialize();
}

//-----------------------------------//

void MonoRuntime::onPluginDisable()
{

}

//-----------------------------------//

void MonoRuntime::onPluginUpdate()
{
}

//-----------------------------------//

static Allocator* g_MonoAlloc = 0;
static GMemVTable g_MonoVTable;
 
static gpointer mono_malloc (gsize n_bytes)
{
	assert(g_MonoAlloc && "Expected a valid allocator");
	return g_MonoAlloc->allocate(g_MonoAlloc, n_bytes, 0);
}

static gpointer mono_realloc (gpointer mem, gsize n_bytes)
{
	assert(g_MonoAlloc && "Expected a valid allocator");

	g_MonoAlloc->deallocate(g_MonoAlloc, mem);
	return g_MonoAlloc->allocate(g_MonoAlloc, n_bytes, 0);
}

static void mono_free (gpointer mem)
{
	assert(g_MonoAlloc && "Expected a valid allocator");
	return g_MonoAlloc->deallocate(g_MonoAlloc, mem);
}

static gpointer mono_calloc (gsize n_blocks, gsize n_block_bytes)
{
	return mono_malloc(n_blocks * n_block_bytes);
}

static gpointer mono_try_malloc (gsize n_bytes)
{
	return mono_malloc(n_bytes);
}

static gpointer mono_try_realloc (gpointer mem, gsize n_bytes)
{
	return mono_realloc(mem, n_bytes);
}

void MonoRuntime::initialize()
{
	heap = AllocatorCreateHeap( AllocatorGetHeap() );

	// Initialize the memory hooks.
	g_MonoVTable.malloc = mono_malloc;
	g_MonoVTable.realloc = mono_realloc;
	g_MonoVTable.free = mono_free;
	g_MonoVTable.calloc = mono_calloc;
	g_MonoVTable.try_malloc = mono_try_malloc;
	g_MonoVTable.try_realloc = mono_try_realloc;
	//g_mem_set_vtable(&g_MonoVTable);

	//Path dir = PathCombine(PathGetCurrentDir(), "Plugins");
	//mono_set_dirs(dir.c_str(), dir.c_str());

	// Load the default Mono configuration file.
	mono_config_parse(nullptr);

	// Creates a new domain where each assembly is loaded and run.
	domain = mono_jit_init_version("Runtime", "v2.0.50727");

	if (!domain)
	{
		LogError("Could not initialize the Mono runtime.");
		return;
	}

	const char* error = mono_check_corlib_version();
	
	if(error)
	{
		LogError("Corlib not in sync with this runtime: %s", error);
		return;
	}

#ifdef PLATFORM_WINDOWS
	gs_MonoUnhandledExceptionFilter = GetUnhandledExceptionFilter();
#endif
}

//-----------------------------------//

void MonoRuntime::cleanup()
{
	// Cleanup the Mono JIT context.
	if (domain)
		mono_jit_cleanup(domain);

	//mono_config_cleanup();

	//g_mem_set_vtable(nullptr);
}

//-----------------------------------//

void MonoRuntime::call(MonoMethod* method)
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
	__except ( gs_MonoUnhandledExceptionFilter( GetExceptionInformation() ) )
	{
	}
#endif

	if( exception != nullptr )
	{
		MonoString* trace = mono_object_to_string(exception, nullptr);
		assert( trace && "Expected to get a valid stack trace" );

		LogError("Exception from managed code:\n%s", mono_string_to_utf8(trace));
		return;
	}
}

//-----------------------------------//

NAMESPACE_RUNTIME_END