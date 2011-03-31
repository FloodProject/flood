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

FWD_DECL_INTRUSIVE(Material)
FWD_DECL_INTRUSIVE(Renderable)

namespace vapor {

//-----------------------------------//

/**
 * Builds the mesh into a ready to render format.
 */

class MeshBuilder
{
public:

	MeshBuilder();

	// Builds the mesh.
	bool build(const MeshPtr& mesh);

	// Builds the mesh geometry data.
	void buildGeometry();

	// Builds the mesh skeleton data.
	void buildSkeleton();

	static std::map<MeshPtr, std::vector<RenderablePtr> > meshRenderables;

protected:

	MeshPtr mesh;

	// Builds the material for a group.
	MaterialPtr buildMaterial( const MeshGroup& group );
};

//-----------------------------------//

} // end namespace