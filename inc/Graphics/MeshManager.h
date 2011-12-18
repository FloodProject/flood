/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Stream.h"
#include "Graphics/Renderable.h"
#include "Resources/Mesh.h"
#include "Resources/Animation.h"
#include "Resources/Skeleton.h"
#include "Resources/Material.h"

FWD_DECL_INTRUSIVE(Renderable)
FWD_DECL_INTRUSIVE(Mesh)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class ResourceManager;
struct ResourceEvent;

typedef std::map<Mesh*, RenderablesVector> MeshRenderablesMap;

/**
 * Responsible for setting up meshes in the render context.
 */

class API_ENGINE MeshManager
{
public:

	MeshManager();
	~MeshManager();

	// Gets a the renderables of a given mesh.
	bool getMeshRenderables( Mesh*, RenderablesVector& );

	// Removes the texture backed by the image.
	void removeMesh( Mesh* );

	// Updates the texture manager.
	void update( float delta );

protected:

	// Populates a texture when the image is loaded.
	void onLoaded( const ResourceEvent& event );

	// Removes a texture when the image file is unloaded.
	void onUnloaded( const ResourceEvent& event );

	// Reloads a texture when the image file changes.
	void onReloaded( const ResourceEvent& event );

	// Builds the mesh.
	bool build(Mesh* mesh);

	// Builds the mesh geometry data.
	void buildGeometry(Mesh* mesh);

	// Builds the material for a group.
	MaterialHandle buildMaterial( Mesh* mesh, const MeshGroup& group );

	// Maps the meshes to renderables.
	MeshRenderablesMap meshRenderables;
};

//-----------------------------------//

NAMESPACE_ENGINE_END