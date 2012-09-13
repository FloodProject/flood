/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Runtime/API.h"
#include "Core/Plugin.h"

#include <mono/metadata/metadata.h>

NAMESPACE_RUNTIME_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(MonoScript)

class MonoScript
{
	MonoAssembly* assembly;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(MonoRuntime)

class MonoRuntime : public Plugin
{
	//REFLECT_DECLARE_OBJECT(MonoRuntime)

public:

	MonoRuntime();

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;
	void onPluginUpdate() OVERRIDE;

	// Initializes the VM.
	void initialize();

	// Cleans up the VM.
	void cleanup();

	// Loads a new assembly in the VM.
	void loadAssembly();

	// Calls a Mono method.
	void call(MonoMethod*);

	Allocator* heap;
	MonoDomain* domain;
};

//-----------------------------------//

NAMESPACE_RUNTIME_END
