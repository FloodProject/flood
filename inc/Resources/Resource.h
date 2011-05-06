/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Object.h"
#include "Core/Reflection.h"
#include "Core/ReferenceCount.h"
#include "Core/Handle.h"

namespace vapor {

//-----------------------------------//

/**
 * Resources can be loaded in a background task. In that case the caller
 * will still receive a resource but it won't be fully loaded. It will
 * only be fully loaded when the resource status changes to loaded.
 */

struct RESOURCE_API ResourceStatus
{
	enum Enum
	{
		Error = 0,
		Unloaded,
		Loading,
		Loaded
	};
};

REFLECT_DECLARE_ENUM(ResourceStatus)

//-----------------------------------//

/**
 * Resource groups identify the kind of data a resource holds.
 * The resource manager class uses this information to keep the
 * related resources together, and for managing the data.
 */

struct RESOURCE_API ResourceGroup
{
	enum Enum
	{
		General = 0,
		Images,
		Meshes,
		Fonts,
		Shaders,
		Audio,
		Scripts,
		Scenes,
		Materials
	};
};

REFLECT_DECLARE_ENUM(ResourceGroup)

//-----------------------------------//

/**
 * Represents a generic resource that tipically is used to hold a piece
 * of data that is used by the various systems of the engine, for example
 * mesh, sounds, fonts, images, shaders, and other types of resource data.
 * Each resource is identified by a path (Uniform Resource Identifier),
 * that way we can add virtual resources (it could be used for various
 * things, like procedural content generation and to identify streaming
 * resource from a network connection).
 */

REFLECT_DECLARE_CLASS(Resource)

class RESOURCE_API Resource : public Object, public ReferenceCounted
{
	DECLARE_UNCOPYABLE(Resource)
	REFLECT_DECLARE_OBJECT(Resource)

public:

	virtual ~Resource() {}

	// Gets/sets the path that identifies this resource.
	ACESSOR(Path, const String&, path)

	// Gets/sets the resource loading status.
	ACESSOR(Status, ResourceStatus::Enum, status)

	// Gets if the resource is fully loaded.
	bool isLoaded() const;

	// Gets the resource group associated with this resource.
	virtual ResourceGroup::Enum getResourceGroup() const = 0;

	// Uniform Resource Identifier.
	String path;

	// Status of the resource.
	ResourceStatus::Enum status;

protected:

	Resource();
};

void* ResourceHandleFind(HandleId id);
void  ResourceHandleDestroy(HandleId id);

#define TYPEDEF_RESOURCE_HANDLE_FROM_TYPE(T) \
	typedef Handle<T, ResourceHandleFind, ResourceHandleDestroy> T##Handle;

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE(Resource);

ResourceHandle ResourceHandleCreate(Resource*);

//-----------------------------------//

} // end namespace


