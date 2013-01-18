/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Resources/API.h"
#include "Core/Object.h"
#include "Core/Reflection.h"
#include "Core/References.h"
#include "Core/Handle.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

/**
 * Resources can be loaded in a background task. In that case the caller
 * will still receive a resource but it won't be fully loaded. It will
 * only be fully loaded when the resource status changes to loaded.
 */

enum class ResourceStatus
{
	Error = 0,
	Unloaded,
	Loading,
	Loaded
};

API_RESOURCE REFLECT_DECLARE_ENUM(ResourceStatus)

//-----------------------------------//

/**
 * Resource groups identify the kind of data a resource holds.
 * The resource manager class uses this information to keep the
 * related resources together, and for managing the data.
 */

enum class ResourceGroup
{
	General = 0,
	Images,
	Meshes,
	Fonts,
	Shaders,
	Audio,
	Scripts,
	Scenes,
	Materials,
	Particles,
};

API_RESOURCE REFLECT_DECLARE_ENUM(ResourceGroup)

//-----------------------------------//

API_RESOURCE REFLECT_DECLARE_CLASS(Resource)

struct ResourceStream;

/**
 * Represents a generic resource that tipically is used to hold a piece
 * of data that is used by the various systems of the engine, for example
 * mesh, sounds, fonts, images, shaders, and other types of resource data.
 * Each resource is identified by a path (Uniform Resource Identifier),
 * that way we can add virtual resources (it could be used for various
 * things, like procedural content generation and to identify streaming
 * resource from a network connection).
 */

class API_RESOURCE Resource : public Object
{
	DECLARE_UNCOPYABLE(Resource)
	REFLECT_DECLARE_OBJECT(Resource)

public:

	virtual ~Resource();

	/// Gets/sets the path that identifies this resource.
	ACCESSOR(Path, const Path&, path)

	/// Gets/sets the resource loading status.
	ACCESSOR(Status, ResourceStatus, status)

	/// Gets if the resource is fully loaded.
	bool isLoaded() const;

	/// Gets the resource group associated with this resource.
	virtual ResourceGroup getResourceGroup() const = 0;

	/// Path to the resource.
	Path path;

	/// Status of the resource.
	ResourceStatus status;

	/// Resource stream.
	ResourceStream* stream;

protected:

	Resource();
};

API_RESOURCE ReferenceCounted* ResourceHandleFind(HandleId id);
API_RESOURCE void  ResourceHandleDestroy(HandleId id);

#define RESOURCE_HANDLE_TYPE(T) \
	Handle<T, ResourceHandleFind, ResourceHandleDestroy>

#define TYPEDEF_RESOURCE_HANDLE_FROM_TYPE(T) \
	typedef RESOURCE_HANDLE_TYPE(T) T##Handle;

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE(Resource);

API_RESOURCE ResourceHandle ResourceHandleCreate(Resource*);

//-----------------------------------//

NAMESPACE_RESOURCES_END

