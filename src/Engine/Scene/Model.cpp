/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Scene/Model.h"
#include "Engine/Scene/Transform.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Animation.h"
#include "Engine/Resources/Skeleton.h"
#include "Engine/Resources/Bone.h"
#include "Engine/Resources/Attachment.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/RenderContext.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/ShaderProgramManager.h"
#include "Core/Math/Helpers.h"
#include "Engine/Geometry/DebugGeometry.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(SkinningMode)
    ENUM(Native)
    ENUM(CPU)
    ENUM(GPU)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(Model, Geometry)
    FIELD_PRIMITIVE(6, float, animationSpeed)
    FIELD_PRIMITIVE(7, bool, animationEnabled)
    FIELD_ENUM_SETTER(8, SkinningMode, skinningMode, SkinningMode)
    FIELD_CLASS_PTR_SETTER(9, Mesh, MeshHandle, meshHandle, Handle, Mesh)
    FIELD_ALIAS(meshHandle, mesh)
REFLECT_CLASS_END()

//-----------------------------------//

Model::MeshRenderablesMap Model::meshRenderables;

Model::Model()
{
}

//-----------------------------------//

Model::~Model()
{
}

//-----------------------------------//

void Model::init()
{
    updateSkin = false;
    modelBuilt = false;
    debugRenderable = nullptr;
    skinningMode = SkinningMode::Native;
    mesh = nullptr;

    animationEnabled = true;
    animationFadeTime = 0;
    animationCurrentFadeTime = 0;
    animationSpeed = 1;

    animations.Clear();
    attachments.Clear();
    bones.Clear();
    renderables.Clear();
}

//-----------------------------------//

void Model::setMesh(const MeshHandle& meshHandle)
{
    for( size_t i = 0; i < renderables.Size(); ++i )
    {
        Renderable* rend = renderables[i].get();
        rend->onPreRender.Bind(this, &Model::onRender);
    }

    init();
    this->meshHandle = meshHandle;
}

//-----------------------------------//

void Model::setSkinningMode( SkinningMode mode )
{
    skinningMode = mode;
    updateSkin = true;
}

//-----------------------------------//

void Model::updateSkinning()
{
    if( !mesh->isAnimated() ) return;
    
    bool isCPU = skinningMode == SkinningMode::CPU;
    String shader = isCPU ? "VertexLit" : "VertexLitSkinned";
    
    for( size_t i = 0; i < renderables.Size(); ++i )
    {
        const RenderBatch* rend = renderables[i].get();
    
        Material* material = rend->getMaterial().Resolve();
        material->setShader(shader);
    }

    //if( !isCPU ) rebuildPositions();
}

//-----------------------------------//

void Model::update( float delta )
{
    mesh = meshHandle.Resolve();

    if( !mesh || !mesh->isLoaded() )
        return;

    if( !modelBuilt ) build();

    if(updateSkin)
    {
        updateSkinning();
        updateSkin = false;
    }

    if( mesh->isAnimated() )
    {
        if( animations.Empty() )
            setAnimation( mesh->getBindPose().get() );

        if( animationEnabled )
        {
            updateAnimations(delta);
            updateFinalAnimationBones();
        }

        updateAttachments();
    }

    //updateDebugRenderable();
    Geometry::update(delta);
}

//-----------------------------------//

void Model::prepareSkinning()
{
    if( !mesh->isAnimated() ) return;

    size_t numBones = mesh->getSkeleton()->getBones().Size();
    bones.Resize( numBones );
}

//-----------------------------------//

bool MeshBuild(Mesh* mesh, RenderablesVector& rends);

void Model::build()
{
    if( modelBuilt ) return;

    MeshRenderablesMap::Iterator it = meshRenderables.Find(mesh);
    
    if( it == meshRenderables.End() )
    {
        RenderablesVector& rends = meshRenderables[mesh];
    
        if( !MeshBuild(mesh, rends) )
        {
            LogError("Error building mesh '%s'", mesh->getPath().CString());
            return;
        }
    }

    RenderablesVector& rends = meshRenderables[mesh];

    for( size_t i = 0; i < rends.Size(); ++i )
    {
        Renderable* rend = rends[i].get();
        rend->onPreRender.Bind(this, &Model::onRender);
        
        addRenderable( rend );
    }

    // Updates the model bounding box.
    updateBounds();
    
    // Re-compute the bounding box.
    Transform* transform = getEntity()->getTransform().get();
    transform->markBoundingVolumeDirty();

    prepareSkinning();

    modelBuilt = true;
}

//-----------------------------------//

void Model::updateAnimations(float delta)
{
    for( size_t i = 0; i < animations.Size(); ++i )
    {
        AnimationState& state = animations[i];

        updateAnimationBones(state);
        updateAnimationTime(state, delta);
    }

    if( animationCurrentFadeTime > animationFadeTime )
    {
        animationFadeTime = 0;
        animationCurrentFadeTime = 0;

        if( animations.Size() >= 2 )
            animations.Erase( animations.begin() + 1 );
    }

    if( animationFadeTime > 0 )
        animationCurrentFadeTime += float(delta);
}

//-----------------------------------//

void Model::updateAnimationTime(AnimationState& state, float delta)
{
    Animation* animation = state.animation.get();
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

    Vector<Matrix4x3>& bones = state.bonesMatrix;
    const KeyFramesMap& keyFrames = animation->getKeyFrames();

    KeyFramesMap::ConstIterator it;
    for( it = keyFrames.Begin(); it != keyFrames.End(); it++ )
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
    if( mesh->animations.Empty() )
    {
        const BonesVector& skeletonBones = mesh->skeleton->getBones();
        
        for( size_t i = 0; i < bones.Size(); ++i )
            bones[i] = skeletonBones[i]->absoluteMatrix;
        
        return;
    }

    for( size_t i = 0; i < bones.Size(); ++i )
    {
        if( animations.Size() >= 2 )
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
    for( size_t i = 0; i < attachments.Size(); ++i )
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

void Model::setAnimation(Animation* animation)
{
    assert( mesh != nullptr );

    if( !mesh->isAnimated() ) return;

    if( !animation ) return;

    AnimationState state;
    state.animation = animation;
    state.animationTime = 0;

    size_t numBones = mesh->getSkeleton()->getBones().Size();
    state.bonesMatrix.Resize(numBones);

    if( animations.Empty() )
        animations.Push(state);
    else
        animations[0] = state;

    animationEnabled = true;
}

//-----------------------------------//

void Model::setAnimation(const String& name)
{
    if( !mesh ) return;

    Animation* animation = mesh->findAnimation(name);
    setAnimation(animation);
}

//-----------------------------------//

void Model::setAnimationFade(const String& name, float fadeTime)
{
    assert( mesh != nullptr );

    if( !mesh->isAnimated() )
        return;

    if( animations.Size() >= 2 )
        animations[1] = animations[0];
    else
        animations.Push( animations[0] );
        
    setAnimation(name);
    animationFadeTime = fadeTime;
    animationCurrentFadeTime = 0;
}

//-----------------------------------//

bool Model::isHardwareSkinned()
{
    RenderPipeline pipeline = GetRenderDevice()->getPipeline();
    
    bool isFixed = pipeline == RenderPipeline::Fixed;
    bool isCPU = skinningMode == SkinningMode::CPU;

    return !(isFixed || isCPU);
}

//-----------------------------------//

void Model::onRender(RenderView* view, const RenderState&)
{
    if( !mesh || !mesh->isLoaded() || !mesh->isAnimated() )
        return;

    if( !isHardwareSkinned() )
    {
        if( renderables.Empty() ) return;

        const RenderBatch* rend = renderables[0].get();
        
        const GeometryBufferPtr& gb = rend->getGeometryBuffer();
        doSkinning(gb);
    }
    else
    {
        setupShaderSkinning();
    }
}

//-----------------------------------//

void Model::doSkinningRaw(Vector3* positions)
{
    GeometryBuffer* gb = mesh->getGeometryBuffer().get();
    size_t numVertices = gb->getNumVertices();

#if 0
    for(size_t i = 0; i < numVertices; ++i)
    {
        int32 boneIndex = (int32) gb->getAttribute(VertexAttribute::BoneIndex, i);

        Vector3* pos = positions + i;
        *pos = bones[boneIndex] * meshPositions[i];
    }
#endif
}

//-----------------------------------//

void Model::doSkinning(const GeometryBufferPtr& gb)
{
    VertexElement* elemPosition = gb->declarations.find(VertexAttribute::Position);
    if( !elemPosition ) return;

    // Can only handle buffers with 3 components and float.
    if(elemPosition->components != 3 || elemPosition->type != VertexDataType::Float)
        return;

#if 0
    uint32 numVertices = gb->getNumVertices();

    if( mesh->position.Size() != numVertices )
    {
        LogDebug("Skinned mesh and its buffer have different vertices");
        return;
    }

    for(size_t i = 0; i < meshPositions.Size(); ++i)
    {
        int32 boneIndex = (int32) mesh->boneIndices[i];

        Vector3* pos = (Vector3*) gb->getAttribute(VertexAttribute::Position, i);
        *pos = bones[boneIndex] * meshPositions[i];
    }
#endif

    gb->forceRebuild();
}

//-----------------------------------//

void Model::setupShaderSkinning()
{
    // Setup matrices.
    Vector<Matrix4x4> matrices;
    matrices.Reserve( bones.Size() );

    for( size_t i = 0; i < bones.Size(); ++i )
    {
        const Matrix4x3& bone = bones[i];
        matrices.Push( Matrix4x4(bone) );
    }

    // Send them to the uniform buffer.
    const Vector<RenderBatchPtr>& rends = getRenderables();

    for( size_t i = 0; i < rends.Size(); ++i )
    {
        const RenderBatch* rend = rends[i].get();
        
        UniformBuffer* ub = rend->getUniformBuffer().get();
        ub->setUniform("vp_BonesMatrix", matrices);
    }
}

//-----------------------------------//

void Model::setAttachment(const String& boneName, const EntityPtr& node)
{
    if( !mesh ) return;

    Skeleton* skeleton = mesh->getSkeleton().get();
    assert( skeleton != nullptr );

    Bone* bone = skeleton->findBone(boneName).get();
    if( !bone ) return;

    Attachment* attachment = AllocateThis(Attachment);
    
    attachment->bone = bone;
    attachment->node = node;

    attachments.Push(attachment);
}

//-----------------------------------//

void Model::onDebugDraw( DebugDrawer& debug, DebugDrawFlags debugFlags )
{
    const Skeleton* skeleton = mesh->getSkeleton().get();
    if( !skeleton ) return;
    
    if( bones.Empty() ) return;

    if( !debugRenderable )
        debugRenderable = createDebugRenderable();

    GeometryBuffer* gb = debugRenderable->getGeometryBuffer().get();

    Vector<Vector3> pos;
    Vector<Vector3> colors;

    size_t numBones = skeleton->bones.Size();
    
    for( size_t i = 0; i < numBones; ++i )
    {
        Bone* bone = skeleton->bones[i].get();

        Vector3 vertex;
        Vector3 parentVertex;

        if( bone->indexParent == -1 )
            continue;
        
        parentVertex = bones[bone->indexParent]*parentVertex;

        pos.Push( bones[bone->index]*vertex );
        colors.Push( Color::Blue );

        pos.Push( parentVertex);
        colors.Push( Color::Blue );
    }

    gb->set( VertexAttribute::Position, pos );
    gb->set( VertexAttribute::Color, colors );

    debug.renderables.Push(debugRenderable.get());
}

//-----------------------------------//

RenderBatchPtr Model::createDebugRenderable() const
{
    MaterialHandle handleMaterial = MaterialCreate(AllocatorGetHeap(), "SkeletonDebug");

    Material* material = handleMaterial.Resolve();
    material->setDepthTest(false);

    GeometryBuffer* gb = AllocateHeap(GeometryBuffer);
    
    RenderBatch* renderable = AllocateHeap(Renderable);
    renderable->setPrimitiveType(PrimitiveType::Lines);
    renderable->setGeometryBuffer(gb);
    renderable->setMaterial(handleMaterial);

    return renderable;
}

//-----------------------------------//

NAMESPACE_ENGINE_END