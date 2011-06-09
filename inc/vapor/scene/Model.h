/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Geometry.h"
#include "Resources/Mesh.h"

//FWD_DECL_INTRUSIVE(Mesh)
FWD_DECL_INTRUSIVE(Bone)
FWD_DECL_INTRUSIVE(Animation)
FWD_DECL_INTRUSIVE(Attachment)

namespace vapor {

//-----------------------------------//

REFLECT_DECLARE_ENUM(SkinningMode)

struct SkinningMode
{
	enum Enum
	{
		Native,
		CPU,
		GPU
	};
};

struct AnimationState;

/**
 * Models are specific instances of meshes in the scene. This way the
 * engine can share the mesh geometry and render the same model at
 * lots of places in the scene. This takes a mesh resource and waits
 * for it to be fully loaded.
 */

REFLECT_DECLARE_CLASS(Model)

class API_ENGINE Model : public Geometry
{
	REFLECT_DECLARE_OBJECT(Model)

public:

	Model();
	Model( const MeshHandle& mesh );

	// Gets the mesh associated with the model.
	GETTER(Mesh, const MeshHandle&, meshHandle)

	// Gets the skinning mode of the mesh.
	GETTER(SkinningMode, SkinningMode::Enum, skinningMode)
	
	// Gets the skinning mode of the mesh.
	void setSkinningMode( SkinningMode::Enum );

	// Sets the mesh.
	void setMesh(const MeshHandle& mesh);

	// Sets the current animation.
	void setAnimation(const AnimationPtr& animation);

	// Sets the current animation.
	void setAnimation(const String& name);

	// Sets and fades the animation.
	void setAnimationFade(const String& name, float fade = 1.0f);

	// Attaches an entity to the model.
	void setAttachment(const String& bone, const EntityPtr& entity);

	// Returns if the mesh is skinned on the GPU.
	bool isHardwareSkinned();

	// Performs CPU skinning of the mesh.
	void doSkinning(std::vector<Vector3>& newPositions);
	
	// Updates the model.
	void update( float delta );
  
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
	void updateAnimations(float delta);

	// Updates the animation time.
	void updateAnimationTime(AnimationState& state, float delta);

	// Updates the matrices of the bones.
	void updateAnimationBones(AnimationState& state);

	// Updates the final animation matrix.
	void updateFinalAnimationBones();

	// Updates the debug renderable of the skeleton.
	void updateDebugRenderable() const;

	// Creates a debug renderable of the skeleton.
	virtual RenderablePtr createDebugRenderable() const;

	// Pre-render callback.
	void onRender(const RenderState& state);

	// Prepares a mesh for skinning.
	void prepareSkinning();

	// Sets up shader skinning.
	void setupShaderSkinning();

	// Rebuilds the model with the mesh positions.
	void rebuildPositions();

	// Mesh that the model renders.
	MeshHandle meshHandle;

	// Pointer to the mesh.
	Mesh* mesh;

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

	// Forces skinning to be done via CPU.
	SkinningMode::Enum skinningMode;

	// Animation states.
	std::vector<AnimationState> animations;

	// Attachments.
	std::vector<AttachmentPtr> attachments;

	// Final bones matrices.
	std::vector<Matrix4x3> bones;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Model );

//-----------------------------------//

} // end namespace