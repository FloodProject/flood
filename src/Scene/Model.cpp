/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Model.h"
#include "Scene/Transform.h"
#include "Scene/Entity.h"
#include "Resources/Mesh.h"
#include "Resources/Animation.h"
#include "Resources/Skeleton.h"
#include "Resources/Bone.h"
#include "Resources/Attachment.h"
#include "Render/Device.h"
#include "Render/Program.h"
#include "Render/ProgramManager.h"
#include "Math/Helpers.h"
#include "MeshBuilder.h"

namespace vapor {

//-----------------------------------//

REFLECT_ENUM(SkinningMode)
	ENUM(Native)
	ENUM(CPU)
	ENUM(GPU)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(Model, Geometry)
	FIELD_PRIMITIVE(float, animationSpeed)
	FIELD_PRIMITIVE(bool, animationEnabled)
	FIELD_ENUM_SETTER(SkinningMode, skinningMode, SkinningMode)
	FIELD_CLASS_PTR_SETTER(Mesh, MeshHandle, meshHandle, Handle, Mesh)
REFLECT_CLASS_END()

//-----------------------------------//

Model::Model()
{
	init();
}

//-----------------------------------//

Model::Model( const MeshHandle& mesh )
	: meshHandle(mesh)
{
	init();
}

//-----------------------------------//

void Model::init()
{
	modelBuilt = false;
	debugRenderable = nullptr;
	skinningMode = SkinningMode::Native;
	mesh = nullptr;

	animationEnabled = true;
	animationFadeTime = 0;
	animationCurrentFadeTime = 0;
	animationSpeed = 1;

	animations.clear();
	attachments.clear();
	bones.clear();
	renderables.clear();
}

//-----------------------------------//

void Model::setMesh(const MeshHandle& mesh)
{
	for( size_t i = 0; i < renderables.size(); i++ )
	{
		const RenderablePtr& rend = renderables[i];
		rend->onPreRender.Disconnect(this, &Model::onRender);
	}

	init();
	meshHandle = mesh;
}

//-----------------------------------//

void Model::setSkinningMode( SkinningMode::Enum mode )
{
	skinningMode = mode;

	if( !mesh->isAnimated() ) return;
	
	bool isCPU = skinningMode == SkinningMode::CPU;
	String shader = isCPU ? "VertexLit" : "VertexLitSkinned";
	
	for( size_t i = 0; i < renderables.size(); i++ )
	{
		const RenderablePtr& rend = renderables[i];
	
		Material* material = rend->getMaterial().Resolve();
		material->setProgram(shader);
	}

	if( !isCPU ) rebuildPositions();
}

//-----------------------------------//

void Model::update( float delta )
{
	mesh = meshHandle.Resolve();

	if( !mesh || !mesh->isLoaded() )
		return;

	if( !modelBuilt ) build();

	if( mesh->isAnimated() )
	{
		if( animations.empty() )
		{
			setAnimation( mesh->getBindPose() );
		}

		if( animationEnabled )
		{
			updateAnimations(delta);
			updateFinalAnimationBones();
		}

		updateAttachments();
	}

	updateDebugRenderable();
	Geometry::update(delta);
}

//-----------------------------------//

void Model::prepareSkinning()
{
	if( !mesh->isAnimated() ) return;

	size_t numBones = mesh->getSkeleton()->getBones().size();
	bones.resize( numBones );
}

//-----------------------------------//

void Model::rebuildPositions()
{
	if( renderables.empty() ) return;

	const RenderablePtr& rend = renderables[0];
	const VertexBufferPtr& vb = rend->getVertexBuffer();

	const std::vector<Vector3>& meshPositions = mesh->position;
	vb->set( VertexAttribute::Position, meshPositions );
	
	vb->forceRebuild();
}

//-----------------------------------//

void Model::build()
{
	if( modelBuilt ) return;

	const std::vector<RenderablePtr>& renderables = MeshBuilder::meshRenderables[mesh];

	if( renderables.empty() )
	{
		MeshBuilder builder;
		builder.build(meshHandle);
	}

	for( size_t i = 0; i < renderables.size(); i++ )
	{
		const RenderablePtr& rend = renderables[i];
		const MaterialHandle& handle = rend->getMaterial();

		Material* material = handle.Resolve();
		
		if( material && material->isBlendingEnabled() )
			rend->setRenderLayer(RenderLayer::Transparency);

		rend->onPreRender.Connect(this, &Model::onRender);
		
		addRenderable( rend );
	}

	prepareSkinning();

	modelBuilt = true;
}

//-----------------------------------//

void Model::updateAnimations(float delta)
{
	for( size_t i = 0; i < animations.size(); i++ )
	{
		AnimationState& state = animations[i];

		updateAnimationBones(state);
		updateAnimationTime(state, delta);
	}

	if( animationCurrentFadeTime > animationFadeTime )
	{
		animationFadeTime = 0;
		animationCurrentFadeTime = 0;

		if( animations.size() >= 2 )
			animations.erase( animations.begin()+1 );
	}

	if( animationFadeTime > 0 )
		animationCurrentFadeTime += float(delta);
}

//-----------------------------------//

void Model::updateAnimationTime(AnimationState& state, float delta)
{
	const AnimationPtr& animation = state.animation;
	float& animationTime = state.animationTime;
	float totalTime = animation->getTotalTime();

	if( MathFloatCompare(animationTime, totalTime) )
	{
		animationTime = 0;
			
		if( !animation->isLooped() )
			animationEnabled = false;
	}
	else
	{
		animationTime += float(delta * 10 * animationSpeed);

		if( animationTime > totalTime )
		{
			animationTime = animation->getTotalTime();
		}
	}
}

//-----------------------------------//

void Model::updateAnimationBones(AnimationState& state)
{
	const AnimationPtr& animation = state.animation;
	float animationTime = state.animationTime;

	std::vector<Matrix4x3>& bones = state.bonesMatrix;
	const KeyFramesMap& keyFrames = animation->getKeyFrames();

	KeyFramesMap::const_iterator it;
	for( it = keyFrames.begin(); it != keyFrames.end(); it++ )
	{
		const BonePtr& bone = it->first;
		
		const Matrix4x3& matKey = animation->getKeyFrameMatrix(bone, animationTime);	
		Matrix4x3 matBone = matKey * bone->relativeMatrix;

		if( bone->indexParent != -1 )
			bones[bone->index] = matBone * bones[bone->indexParent];
		else
			bones[bone->index] = matBone;
	}
}

//-----------------------------------//

void Model::updateFinalAnimationBones()
{
	if( animations.empty() )
		return;

	for( size_t i = 0; i < bones.size(); i++ )
	{
		if( animations.size() >= 2 )
		{
			bones[i] = Matrix4x3::lerp(
				animations[0].bonesMatrix[i], animations[1].bonesMatrix[i],
				1 - (animationCurrentFadeTime / animationFadeTime));
		}
		else
		{
			bones[i] = animations[0].bonesMatrix[i];
		}
	}
}

//-----------------------------------//

void Model::updateAttachments()
{
	for( size_t i = 0; i < attachments.size(); i++ )
	{
		const AttachmentPtr& attachment = attachments[i];
		const EntityPtr& node = attachment->node;
		const BonePtr& bone = attachment->bone;

		const TransformPtr& transform = node->getTransform();
		transform->setAbsoluteTransform( bones[bone->index] );
	}
}

//-----------------------------------//

void Model::updateBounds()
{
	bounds = mesh->getBoundingVolume();
}

//-----------------------------------//

void Model::setAnimation(const AnimationPtr& animation)
{
	assert( mesh != nullptr );

	if( !mesh->isAnimated() )
		return;

	if( !animation )
		return;

	AnimationState state;
	state.animation = animation;
	state.animationTime = 0;

	size_t numBones = mesh->getSkeleton()->getBones().size();
	state.bonesMatrix.resize(numBones);

	if( animations.empty() )
		animations.push_back(state);
	else
		animations[0] = state;

	animationEnabled = true;
}

//-----------------------------------//

void Model::setAnimation(const std::string& name)
{
	if( !mesh ) return;

	AnimationPtr animation = mesh->findAnimation(name);
	setAnimation(animation);
}

//-----------------------------------//

void Model::setAnimationFade(const std::string& name, float fadeTime)
{
	assert( mesh != nullptr );

	if( !mesh->isAnimated() )
		return;

	if( animations.size() >= 2 )
		animations[1] = animations[0];
	else
		animations.push_back( animations[0] );
		
	setAnimation(name);
	animationFadeTime = fadeTime;
	animationCurrentFadeTime = 0;
}

//-----------------------------------//

bool Model::isHardwareSkinned()
{
	RenderPipeline::Enum pipeline = GetRenderDevice()->getPipeline();
	
	bool isFixed = pipeline == RenderPipeline::Fixed;
	bool isCPU = skinningMode == SkinningMode::CPU;

	return !(isFixed || isCPU);
}

//-----------------------------------//

void Model::onRender()
{
	if( !mesh || !mesh->isLoaded() || !mesh->isAnimated() )
		return;

	if( !isHardwareSkinned() )
	{
		if( renderables.empty() ) return;

		const RenderablePtr& rend = renderables[0];
		const VertexBufferPtr& vb = rend->getVertexBuffer();

		std::vector<Vector3>& skinnedPositions = vb->getVertices();
		doSkinning(skinnedPositions);
		
		vb->forceRebuild();
	}
	else
	{
		setupShaderSkinning();
	}
}

//-----------------------------------//

void Model::doSkinning(std::vector<Vector3>& skinnedPositions)
{
	const std::vector<Vector3>& meshPositions = mesh->position;
	skinnedPositions.resize( meshPositions.size() );

	for(size_t j = 0; j < meshPositions.size(); j++)
	{
		int32 boneIndex = (int32) mesh->boneIndices[j];
		skinnedPositions[j] = bones[boneIndex]*meshPositions[j];
	}
}

//-----------------------------------//

void Model::setupShaderSkinning()
{
	const std::vector<RenderablePtr>& rends = getRenderables();

	for( size_t i = 0; i < rends.size(); i++ )
	{
		const RenderablePtr& rend = rends[i];
		const MaterialHandle& handle = rend->getMaterial();

		Material* material = handle.Resolve();
		if( !material ) return;

		const String& name = material->getProgram();
		ProgramPtr program = GetRenderDevice()->getProgramManager()->getProgram(name);

		std::vector<Matrix4x4> matrices;
		matrices.reserve( bones.size() );

		for( size_t i = 0; i < bones.size(); i++ )
		{
			const Matrix4x3& bone = bones[i];
			matrices.push_back( Matrix4x4(bone) );
		}
		
		program->setUniform("vp_BonesMatrix", matrices);
	}
}

//-----------------------------------//

void Model::setAttachment(const std::string& boneName, const EntityPtr& node)
{
	if(!mesh)
		return;

	SkeletonPtr skeleton = mesh->getSkeleton();
	assert( skeleton != nullptr );

	BonePtr bone = skeleton->findBone(boneName);
	
	if( !bone )
		return;

	AttachmentPtr attachment = new Attachment();
	
	attachment->bone = bone;
	attachment->node = node;

	attachments.push_back(attachment);
}

//-----------------------------------//

void Model::updateDebugRenderable() const
{
	if( !debugRenderable )
		return;

	if( !mesh->isAnimated() )
		return;

	VertexBufferPtr vb = debugRenderable->getVertexBuffer();

	std::vector<Vector3> pos;
	std::vector<Vector3> colors;

	const SkeletonPtr& skel = mesh->getSkeleton();

	uint numBones = skel->bones.size();
	for( size_t i = 0; i < numBones; i++ )
	{
		BonePtr& bone = skel->bones[i];

		Vector3 vertex;
		Vector3 parentVertex;

		if( bone->indexParent == -1 )
			continue;
		
		parentVertex = bones[bone->indexParent]*parentVertex;	

		pos.push_back( bones[bone->index]*vertex );
		colors.push_back( Color::Blue );

		pos.push_back( parentVertex);
		colors.push_back( Color::Blue );
	}

	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );
}

//-----------------------------------//

RenderablePtr Model::createDebugRenderable() const
{
	assert( !debugRenderable );

	MaterialHandle handleMaterial = MaterialCreate(AllocatorGetHeap(), "SkeletonDebug");

	Material* material = handleMaterial.Resolve();
	material->setDepthTest(false);

	VertexBufferPtr vb = Allocate(VertexBuffer, AllocatorGetHeap());
	
	RenderablePtr renderable = Allocate(Renderable, AllocatorGetHeap(), PolygonType::Lines);
	renderable->setVertexBuffer(vb);
	renderable->setMaterial(handleMaterial);

	return renderable;
}

//-----------------------------------//

} // end namespace