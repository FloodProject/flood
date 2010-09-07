/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"
#include "vapor/math/BoundingBox.h"

FWD_DECL_INTRUSIVE(Skeleton)
FWD_DECL_INTRUSIVE(Animation)
FWD_DECL_INTRUSIVE(Renderable)

namespace vapor {

//-----------------------------------//

/**
 * Mesh resources contain 3D geometry data.
 */

class Mesh : public Resource
{
	DECLARE_CLASS_()

public:

	// Builds the geometry of the mesh.
	virtual void build() = 0;
	
	// Gets if the mesh is built.
	bool isBuilt() const;

	// Gets if the mesh is animated.
	bool isAnimated() const;

	// Gets the skeleton of the mesh.
	GETTER(Skeleton, SkeletonPtr, skeleton)

	// Finds an animation from the mesh.
	AnimationPtr findAnimation( const std::string& name );

	// Gets the bounding box of the mesh.
	GETTER(BoundingVolume, const BoundingBox&, boundingVolume)

	// Gets the animations of the mesh.
	GETTER(Animations, const std::vector<AnimationPtr>&, animations)

	// Gets the renderables of the mesh.
	void appendRenderables( std::vector<RenderablePtr>& renderables );

	// Gets the resource group of this resource.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)	

protected:

	Mesh();

	// Keeps track if the mesh is animated.
	bool animated;

	// Skeleton of the mesh.
	SkeletonPtr skeleton;

	// Skeleton of the mesh.
	std::vector<AnimationPtr> animations;

	// Renderables of the mesh.
	std::vector<RenderablePtr> renderables;

	// Bounding volume of the mesh.
	BoundingBox boundingVolume;

	// Keeps track if the mesh has been built.
	bool built;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Mesh );

//-----------------------------------//

} // end namespace