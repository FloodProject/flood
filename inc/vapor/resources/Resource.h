/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

class ResourceLoader;

//-----------------------------------//

/**
 * When you request a resource, you will be given a resource handle that
 * you can later use to query the resource manager for the real pointer.
 * Never store the dumb pointers, because if the resource is updated it
 * might have a different pointer.
 */

typedef int	ResourceHandle; // Note: unused for now.

//-----------------------------------//

/**
 * Resources can be loaded in a background task. In that case the caller
 * will still receive a resource but it won't be fully loaded. It will
 * only be fully loaded when the resource status changes to loaded.
 */

namespace ResourceStatus
{
	enum Enum
	{
		Error = 0,
		Unloaded,
		Loading,
		Loaded
	};
}

//-----------------------------------//

/**
 * Resource groups identify the kind of data a resource holds.
 * The ResourceManager class uses this information to keep the
 * related resources together, and for managing the data.
 */

namespace ResourceGroup
{
	enum Enum
	{
		General = 0,
		Images,
		Meshes,
		Fonts,
		Shaders,
		Audio,
		Scripts
	};

	DECLARE_ENUM()
}

//-----------------------------------//

/**
 * Represents a generic resource that tipically is used to hold a piece
 * of data that is used by the various systems of the engine, for example
 * mesh, sounds, fonts, images, shaders, and other types of resource data.
 * Each resource is identified by an Path (Uniform Resource Identifier),
 * that way we can add virtual resources (it could be used for various
 * things, like procedural content generation and to identify streaming
 * resource from a network connection).
 */

class VAPOR_API Resource : public ReferenceCounted, private boost::noncopyable
{
	DECLARE_CLASS_()

public:

	virtual ~Resource() {}

	// Reloads this resource.
	bool reload();

	// Releases the contents of this resource (to save memory).
	// void release();

	// Gets/sets the path that identifies this resource.
	ACESSOR(Path, const std::string&, path)

	// Returns the base part of the Path.
	std::string getBasePath() const;
	
	// Gets/sets the resource loading status.
	ACESSOR(Status, ResourceStatus::Enum, status)

	// Gets if the resource is fully loaded.
	bool isLoaded() const;

	// Gets the resource group associated with this resource.
	virtual ResourceGroup::Enum getResourceGroup() const = 0;

protected:

	Resource();

	// Uniform Resource Identifier.
	std::string path;

	// Status of the resource.
	ResourceStatus::Enum status;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Resource );

//-----------------------------------//

#define RESOURCE_TYPEDECL_FROM_TYPE(T)			\
	boost::intrusive_ptr<T>

#define RESOURCE_SMART_PTR_CAST					\
	boost::static_pointer_cast

//-----------------------------------//

} // end namespace


