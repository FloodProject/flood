/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Mesh.h"
#include "vapor/animation/Animation.h"
#include "vapor/animation/Skeleton.h"
#include "vapor/vfs/File.h"

FWD_DECL_INTRUSIVE(Material)

namespace vapor {

//-----------------------------------//

struct ms3d_vertex_t;
struct ms3d_triangle_t;
struct ms3d_group_t;
struct ms3d_material_t;
struct ms3d_joint_t;
struct ms3d_keyframe_t;

struct AnimationMetadata
{
	int start;
	int end;
	std::string name;
	float startTime;
};

//-----------------------------------//

/**
 * Represents Milkshape 3D meshes. These can hold vertex, animation, and 
 * material data, are relatively easy to parse and use, and are quite
 * well documented and lightweight.
 */

class Milkshape3D : public Mesh
{
public:

	Milkshape3D();

	// Loads the mesh data into the resource.
	bool load(const File& file);

	// Parses the contents of the file.
	bool parse();

	// Builds the mesh.
	void build();

	// Builds the mesh geometry data.
	void buildGeometry();

	// Builds the mesh bounding volume.
	void buildBounds();

	// Builds the mesh skeleton data.
	void buildSkeleton();

	// Builds the mesh animation data.
	void buildAnimations();

	// Builds an animation from a metadata.
	AnimationPtr buildAnimation(AnimationMetadata& data);

	// Builds the animation metadata.
	void buildAnimationMetadata();

	// Builds the key frame data for a joint.
	void buildKeyFrames( const ms3d_joint_t& joint,
		const AnimationMetadata& data,KeyFramesVector& frames );

protected:

	// Reads the meshes structures from the data.
	bool readHeader();
	void readVertices();
	void readTriangles();
	void readGroups();
	void readMaterials();
	void readAnimation();
	void readJoints();
	void readComments();

	// Builds the material for a group.
	MaterialPtr buildMaterial( const ms3d_group_t& group );

	// Gets the start of the animation.
	float getAnimationStart(const AnimationMetadata& data);

	// Returns if a group has a material.
	bool hasMaterial( const ms3d_group_t& group );

	// Sets up the joints hierarchy indices.
	void setupJointsHierarchy();

	// Sets up the joint matrices.
	void setupJointMatrices();

	// Sets up the joint rotations.
	void setupJointRotations();

	// Sets up the initial vertices in joint-space.
	void setupInitialVertices();

	// Finds a joint by name.
	int findJoint(const char* name);

	// Contents of the file.
	std::vector<byte> filebuf;
	long index;

	// Mesh data structures.
	std::vector<ms3d_vertex_t*> vertices;
	std::vector<ms3d_triangle_t*> triangles;
	std::vector<ms3d_group_t> groups;
	std::vector<ms3d_material_t*> materials;
	std::vector<ms3d_joint_t> joints;
	std::string mainComment;

	// Animation metadata.
	std::vector<AnimationMetadata> metadata;

	// Animation FPS.
	float animationFPS;

	// Total frames of animation.
	int totalFrames;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Milkshape3D );

//-----------------------------------//

} // end namespace