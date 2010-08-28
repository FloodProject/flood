/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Geometry.h"

FWD_DECL_INTRUSIVE(Mesh)
FWD_DECL_INTRUSIVE(Bone)
FWD_DECL_INTRUSIVE(Animation)

namespace vapor {

//-----------------------------------//

/**
 * Models are specific instances of meshes in the scene. This way the
 * engine can share the mesh geometry and render the same model at
 * lots of places in the scene. This takes a mesh resource and waits
 * for it to be fully loaded.
 */

class VAPOR_API Model : public Geometry
{
	DECLARE_CLASS_()

public:

	Model();
	Model( const MeshPtr& mesh );

	// Updates the mesh.
	void update( double delta );
  
	// Builds the mesh when it is fully loaded.
	void build();

protected:

	// Initializes the model.
	void init();

	// Updates the animation of the model.
	void updateAnimation();

	// Updates the matrices of the bones.
	void updateBoneMatrices();

	// Updates the matrix of a bone.
	void updateBoneMatrix(const BonePtr& bone);

	// Gets a debug renderable of the skeleton.
	virtual RenderablePtr getDebugRenderable() const;

	// Creates a debug renderable of the skeleton.
	virtual void createDebugRenderable() const;

	// Callback when geometry is about to get rendered.
	virtual void onRender();

	// Mesh that the model renders.
	MeshPtr mesh;

	// Has the model been built.
	bool modelBuilt;

	// Current animation.
	AnimationPtr animation;

	// Keeps track if we want to perform animation.
	bool animate;

	// Current animation time.
	float currentTime;

	// Bones matrices.
	mutable std::vector<Matrix4x3> bonesMatrix;

	// Mesh that the model renders.
	mutable RenderablePtr renderable;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Model );

//-----------------------------------//

} // end namespace