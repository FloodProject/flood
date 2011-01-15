/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "scene/Geometry.h"
#include "resources/Mesh.h"

//FWD_DECL_INTRUSIVE(Mesh)
FWD_DECL_INTRUSIVE(Bone)
FWD_DECL_INTRUSIVE(Animation)
FWD_DECL_INTRUSIVE(Attachment)

namespace vapor {

//-----------------------------------//

struct AnimationState;

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

	// Gets the mesh associated with the model.
	GETTER(Mesh, const MeshPtr&, mesh)

	// Sets the mesh animation.
	void setAnimation(const AnimationPtr& animation);

	// Sets the mesh animation.
	void setAnimation(const std::string& name);

	// Sets and fades the mesh animation.
	void setAnimationFade(const std::string& name, float fade = 1.0f);

	// Attaches a node to the bone.
	void setAttachment(const std::string& bone, const EntityPtr& node);

	// Updates the mesh.
	void update( double delta );
  
	// Builds the mesh when it is fully loaded.
	void build();

protected:

	// Initializes the model.
	void init();

	// Updates the geometry bounds if needed.
	virtual void updateBounds();

	// Updates the attachments of the model.
	void updateAttachments();

	// Updates the animation of the model.
	void updateAnimations(double delta);

	// Updates the animation time.
	void updateAnimationTime(AnimationState& state, double delta);

	// Updates the matrices of the bones.
	void updateAnimationBones(AnimationState& state);

	// Updates the final animation matrix.
	void updateFinalAnimationBones();

	// Updates the debug renderable of the skeleton.
	void updateDebugRenderable() const;

	// Creates a debug renderable of the skeleton.
	virtual RenderablePtr createDebugRenderable() const;

	// Pre-render callback.
	void onRender();

	// Sets up CPU skinning.
	void setupSkinning();

	// Sets up shader skinning.
	void setupShaderSkinning();

	// Mesh that the model renders.
	MeshPtr mesh;

	// Has the model been built.
	bool modelBuilt;

	// Keeps track if we want to perform animation.
	bool animationEnabled;

	// Animation fade time.
	float animationFadeTime;

	// Animation fade time.
	float animationCurrentFadeTime;

	// Controls animation speed.
	float animationSpeed;

	// Animation states.
	std::vector<AnimationState> animations;

	// Attachments.
	std::vector<AttachmentPtr> attachments;

	// Final bones matrices.
	std::vector<Matrix4x3> bones;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Model );

//-----------------------------------//

} // end namespace