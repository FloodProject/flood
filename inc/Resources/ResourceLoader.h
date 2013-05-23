/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Stream.h"
#include "Core/Extension.h"
#include "Core/Memory.h"
#include "Resources/Resource.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

struct API_RESOURCE ResourceLoadOption
{
	ResourceLoadOption() : key(0), value(0) {}

	int key;
	int value;
};

//-----------------------------------//

struct API_RESOURCE ResourceLoadOptions
{
	ResourceLoadOptions();

	// Adds a resource load option.
	void addOption(int key, int value);

	String name;
	Stream* stream;

	Resource* resource;
	ResourceGroup group;

	bool isHighPriority;
	bool sendLoadEvent;
	bool asynchronousLoad;
	bool keepStreamOpen;

	ResourceLoadOption option;
};

//-----------------------------------//

class ResourceLoader;

struct API_RESOURCE ResourceStream
{
	ResourceStream();
	virtual ~ResourceStream();

	// Streams a chunk of the resource.
	virtual int decode(uint8* buffer, size_t size) = 0;

	// Resets the stream to the beginning.
	virtual void reset() = 0;

	Stream* stream;
	ResourceLoader* loader;
};

//-----------------------------------//

#define RESOURCE_LOADER_PREPARE(T) \
	Resource* prepare(ResourceLoadOptions&) OVERRIDE { return AllocateThis(T); }

#define RESOURCE_LOADER_CLASS(T) \
	Class* getResourceClass() const OVERRIDE { return ReflectionGetType(T); }

/**
 * Resource loaders decode resource data into a proper resource.
 * This is an interface that should be implemented to provide the
 * resource codec services for the formats handled by the loader.
 * The loader also provides the extensions of those formats.
 */

API_RESOURCE REFLECT_DECLARE_CLASS(ResourceLoader)

class API_RESOURCE ResourceLoader : public Extension
{
	DECLARE_UNCOPYABLE(ResourceLoader)
	REFLECT_DECLARE_OBJECT(ResourceLoader)

public:

	ResourceLoader();
	virtual ~ResourceLoader();

	// Gets metadata about this extension.
	virtual ExtensionMetadata* getMetadata() OVERRIDE;

	// Creates the resource with no data (no I/O here).
	virtual Resource* prepare(ResourceLoadOptions&) = 0;

	// Decodes a given file into a resource.
	virtual bool decode(ResourceLoadOptions&) = 0;

	// Gets the name of this loader.
	virtual const String getName() const = 0;

	// Gets the resource class of this loader.
	virtual Class* getResourceClass() const = 0;

	// Gets the resource group of this loader.
	virtual ResourceGroup getResourceGroup() const = 0;

	// Gets the recognized extensions of this loader.
	GETTER(Extensions, const Array<String*>&, extensions);

	Array<String*> extensions;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(ResourceLoader)

//-----------------------------------//

NAMESPACE_RESOURCES_END