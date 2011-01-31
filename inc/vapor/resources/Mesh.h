/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"
#include "Resources/Animation.h"
#include "Resources/Skeleton.h"
#include "Math/BoundingBox.h"
#include "Math/Vector3.h"

FWD_DECL_INTRUSIVE(Animation)
FWD_DECL_INTRUSIVE(Renderable)

namespace vapor {

//-----------------------------------//

struct MeshMaterial
{
	std::string name;
	std::string texture;
	bool alpha;
};

struct MeshGroup
{
	std::vector<Vector3> position;
	std::vector<Vector3> normals;
	std::vector<Vector3> texCoords;
	std::vector<float> bones;
	MeshMaterial material;
};

/**
 * Mesh resources contain 3D geometry data.
 */

class RESOURCE_API Mesh : public Resource
{
	DECLARE_CLASS_()

public:

	Mesh();

	// Gets if the mesh is built.
	bool isBuilt() const;

	// Gets if the mesh is animated.
	bool isAnimated() const;

	// Gets the skeleton of the mesh.
	GETTER(Skeleton, SkeletonPtr, skeleton)

	// Gets the bind pose animation of the mesh.
	GETTER(BindPose, AnimationPtr, bindPose)

	// Finds an animation from the mesh.
	AnimationPtr findAnimation( const std::string& name );

	// Gets the bounding box of the mesh.
	GETTER(BoundingVolume, const BoundingBox&, boundingVolume)

	// Gets the animations of the mesh.
	GETTER(Animations, const std::vector<AnimationPtr>&, animations)

	// Gets the resource group of this resource.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)

public:

	// Sets up the initial vertices in joint-space.
	void setupInitialVertices();

	// Builds the mesh bounding volume.
	void buildBounds();

	// Keeps track if the mesh is animated.
	bool animated;

	// Skeleton of the mesh.
	SkeletonPtr skeleton;

	// Bind pose animation of the mesh.
	AnimationPtr bindPose;

	// Animations of the mesh.
	std::vector<AnimationPtr> animations;

	// Groups of the mesh.
	std::vector<MeshGroup> groups;

	// Bounding volume of the mesh.
	BoundingBox boundingVolume;

	// Renderables of the mesh.
	std::vector<Renderable*> renderables;

	// Vertex data.
	//std::vector<Vector3> position;
	//std::vector<Vector3> normals;
	//std::vector<Vector3> texCoords;
	//std::vector<float> bones;

	// Keeps track if the mesh has been built.
	bool built;

};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Mesh );

//-----------------------------------//

} // end namespace