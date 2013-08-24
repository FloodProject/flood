/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Scene/Geometry.h"

#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Animation.h"
#include "Engine/Resources/Skeleton.h"

FWD_DECL_INTRUSIVE(Bone)
FWD_DECL_INTRUSIVE(Animation)
FWD_DECL_INTRUSIVE(Attachment)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

API_SCENE REFLECT_DECLARE_ENUM(SkinningMode)

enum struct SkinningMode
{
	Native = 0,
	CPU = 1,
	GPU = 2
};

struct AnimationState;

/**
 * Models are specific instances of meshes in the scene. This way the
 * engine can share the mesh geometry and render the same model at
 * lots of places in the scene. This takes a mesh resource and waits
 * for it to be fully loaded.
 */

API_SCENE REFLECT_DECLARE_CLASS(Model)

class API_SCENE Model : public Geometry
{
	REFLECT_DECLARE_OBJECT(Model)

public:

	Model();
	~Model();

	// Gets the mesh associated with the model.
	GETTER(Mesh, const MeshHandle&, meshHandle)

	// Gets the skinning mode of the mesh.
	GETTER(SkinningMode, SkinningMode, skinningMode)
	
	// Gets the skinning mode of the mesh.
	void setSkinningMode( SkinningMode );

	// Sets the mesh.
	void setMesh(const MeshHandle& mesh);

	// Sets the current animation.
	void setAnimation(Animation* animation);

	// Sets the current animation.
	void setAnimation(const String& name);

	// Sets and fades the animation.
	void setAnimationFade(const String& name, float fade = 1.0f);

	// Attaches an entity to the model.
	void setAttachment(const String& bone, const EntityPtr& entity);

	// Returns if the mesh is skinned on the GPU.
	bool isHardwareSkinned();

	// Performs CPU skinning of the mesh.
	void doSkinning(const GeometryBufferPtr& gb);

	// Performs CPU skinning of the mesh.
	void doSkinningRaw(Vector3* positions);

	// Updates the skinning details of the model.
	void updateSkinning();
	
	// Updates the model.
	virtual void update( float delta ) OVERRIDE;

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

	// Called when it is time to draw debug data.
	virtual void onDebugDraw( DebugDrawer&, DebugDrawFlags ) OVERRIDE;

	// Creates a debug renderable of the skeleton.
	RenderBatchPtr createDebugRenderable() const;

	// Pre-render callback.
	void onRender(RenderView* view, const RenderState& state);

	// Prepares a mesh for skinning.
	void prepareSkinning();

	// Sets up shader skinning.
	void setupShaderSkinning();

	// Mesh that the model renders.
	MeshHandle meshHandle;

	// Pointer to the mesh.
	Mesh* mesh;

	// Has the model been built.
	bool modelBuilt;

	// Holds if the model needs skin update.
	bool updateSkin;

	// Keeps track if we want to perform animation.
	bool animationEnabled;

	// Animation fade time.
	float animationFadeTime;

	// Animation fade time.
	float animationCurrentFadeTime;

	// Controls animation speed.
	float animationSpeed;

	// Forces skinning to be done via CPU.
	SkinningMode skinningMode;

	// Animation states.
	Array<AnimationState> animations;

	// Attachments.
	Array<AttachmentPtr> attachments;

	// Final bones matrices.
	Array<Matrix4x3> bones;

	RenderBatchPtr debugRenderable;

	typedef std::map<Mesh*, RenderablesVector> MeshRenderablesMap;
	static MeshRenderablesMap meshRenderables;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Model );

//-----------------------------------//

NAMESPACE_ENGINE_END