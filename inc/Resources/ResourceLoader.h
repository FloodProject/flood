/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Stream.h"
#include "Core/Plugin.h"
#include "Resources/Resource.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

#define RESOURCE_LOADER_PREPARE(T) \
	Resource* prepare(const Stream&) OVERRIDE { return Allocate(T, AllocatorGetThis()); }

#define RESOURCE_LOADER_CLASS(T) \
	Class* getResourceClass() const OVERRIDE { return ReflectionGetType(T); }
/**
 * Resource loaders decode resource data into a proper resource.
 * This is an interface that should be implemented to provide the
 * resource codec services for the formats handled by the loader.
 * The loader also provides the extensions of those formats.
 */

REFLECT_DECLARE_CLASS(ResourceLoader)

class API_RESOURCE ResourceLoader : public Plugin
{
	DECLARE_UNCOPYABLE(ResourceLoader)
	REFLECT_DECLARE_OBJECT(ResourceLoader)

public:

	ResourceLoader() {}
	virtual ~ResourceLoader() {}

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE { return PluginMetadata(); }

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE {}
	void onPluginDisable() OVERRIDE {}

	// Creates the resource with no data (no I/O here).
	virtual Resource* prepare(const Stream&) = 0;

	// Decodes a given file into a resource.
	virtual bool decode(const Stream&, Resource* res) = 0;

	// Gets the name of this loader.
	virtual const String getName() const = 0;

	// Gets the resource class of this loader.
	virtual Class* getResourceClass() const = 0;

	// Gets the resource group of this loader.
	virtual ResourceGroup::Enum getResourceGroup() const = 0;

	// Gets the recognized extensions of this loader.
	GETTER(Extensions, const std::vector<String>&, extensions);

	std::vector<String> extensions;
};

//-----------------------------------//

NAMESPACE_RESOURCES_END