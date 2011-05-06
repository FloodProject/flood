/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Stream.h"
#include "Resources/Mesh.h"
#include "Resources/Animation.h"
#include "Resources/Skeleton.h"
#include "Resources/Material.h"

FWD_DECL_INTRUSIVE(Renderable)

NAMESPACE_BEGIN

//-----------------------------------//

/**
 * Builds the mesh into a ready to render format.
 */

typedef std::map<Mesh*, std::vector<RenderablePtr>> MeshRenderablesMap;

class MeshBuilder
{
public:

	MeshBuilder();

	// Builds the mesh.
	bool build(const MeshHandle& mesh);

	// Builds the mesh geometry data.
	void buildGeometry();

	// Builds the mesh skeleton data.
	void buildSkeleton();

	static MeshRenderablesMap meshRenderables;

protected:

	Mesh* mesh;

	// Builds the material for a group.
	MaterialHandle buildMaterial( const MeshGroup& group );
};

//-----------------------------------//

NAMESPACE_END