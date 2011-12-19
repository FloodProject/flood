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
#include "Resources/Animation.h"
#include "Resources/Skeleton.h"

FWD_DECL_INTRUSIVE(Bone)
FWD_DECL_INTRUSIVE(Animation)
FWD_DECL_INTRUSIVE(Attachment)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_DECLARE_ENUM(SkinningMode)

struct SkinningMode
{
	enum Enum
	{
		Native = 0,
		CPU = 1,
		GPU = 2
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

class API_SCENE Model : public Geometry
{
	REFLECT_DECLARE_OBJECT(Model)

public:

	Model();
	~Model();

	// Gets the mesh associated with the model.
	GETTER(Mesh, const MeshHandle&, meshHandle)

	// Gets the skinning mode of the mesh.
	GETTER(SkinningMode, SkinningMode::Enum, skinningMode)
	
	// Gets the skinning mode of the mesh.
	void setSkinningMode( SkinningMode::Enum );

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
	void update( float delta ) OVERRIDE;

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
	void onDebugDraw( DebugDrawer&, DebugDrawFlags::Enum ) OVERRIDE;

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
	SkinningMode::Enum skinningMode;

	// Animation states.
	std::vector<AnimationState> animations;

	// Attachments.
	std::vector<AttachmentPtr> attachments;

	// Final bones matrices.
	std::vector<Matrix4x3> bones;

	RenderBatchPtr debugRenderable;

	typedef std::map<Mesh*, RenderablesVector> MeshRenderablesMap;
	static MeshRenderablesMap meshRenderables;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Model );

//-----------------------------------//

NAMESPACE_ENGINE_END