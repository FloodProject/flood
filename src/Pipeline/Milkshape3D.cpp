/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

// based on official Milkshape3D viewer source

#include "Pipeline/API.h"
#include "Pipeline/Milkshape3D.h"

#include "Engine/Resources/Skeleton.h"
#include "Graphics/GeometryBuffer.h"

#include "Core/Math/Vector.h"
#include "Core/Math/Helpers.h"
#include "Core/Utilities.h"
#include "Core/Log.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

Milkshape3D::Milkshape3D()
    : index(0)
{ }

//-----------------------------------//

bool Milkshape3D::load(const Stream& stream)
{
    stream.read(filebuf);

    if( filebuf.Empty() ) 
        return false;

    if( !readHeader() )
        return false;
    
    readVertices();
    readTriangles();
    readGroups();
    readMaterials();
    readAnimation();
    readJoints();
    readComments();

    preprocess();
    
    return true;
}

//-----------------------------------//

void Milkshape3D::preprocess()
{
    mesh->animated = !joints.Empty();

    buildGeometry();

    if( !mesh->isAnimated() ) return;
        
    buildSkeleton();
    buildAnimations();
}

//-----------------------------------//

void Milkshape3D::buildSkeleton()
{
    setupJointsHierarchy();
    setupJointRotations();
    setupJointMatrices();

    mesh->skeleton = AllocateThis(Skeleton);
    
    for( size_t i = 0; i < joints.Size(); i++ )
    {
        ms3d_joint_t& joint = joints[i];
        
        Bone* bone = AllocateThis(Bone);

        bone->name = joint.name;
        bone->index = i;
        bone->indexParent = joint.indexParent;
        bone->position = joint.position;
        bone->rotation = joint.rotation;
        bone->relativeMatrix = joint.relativeMatrix;
        bone->absoluteMatrix = joint.absoluteMatrix;
        bone->absoluteInvMatrix = joint.absoluteMatrix.inverse();

        mesh->skeleton->addBone(bone);
    }
}

//-----------------------------------//

void Milkshape3D::setupJointsHierarchy()
{

    for( size_t i = 0; i < joints.Size(); i++ )
    {
        ms3d_joint_t& joint = joints[i];
        joint.indexParent = findJoint(joint.parentName);
    }
}

//-----------------------------------//

int Milkshape3D::findJoint(const char* name)
{
    for( size_t i = 0; i < joints.Size(); i++ )
    {
        const ms3d_joint_t& joint = joints[i];

        if( String::Compare(joint.name, name, false) == 0 )
            return i;
    }

    return -1;
}

//-----------------------------------//

void Milkshape3D::setupJointMatrices()
{
    for( size_t i = 0; i < joints.Size(); i++ )
    {
        ms3d_joint_t& joint = joints[i];

        joint.relativeMatrix =
            Matrix4x3::createRotation(joint.rotation) *
            Matrix4x3::createTranslation(joint.position);

        if( joint.indexParent != -1 )
        {
            const ms3d_joint_t& parent = joints[joint.indexParent];
            joint.absoluteMatrix = joint.relativeMatrix * parent.absoluteMatrix;
        }
        else
        {
            joint.absoluteMatrix = joint.relativeMatrix;
        }
    }
}

//-----------------------------------//

void Milkshape3D::setupJointRotations()
{
    for( size_t i = 0; i < joints.Size(); i++ )
    {
        ms3d_joint_t& joint = joints[i];

        joint.rotation = EulerAngles(
            MathRadianToDegree(joint.rotation.x),
            MathRadianToDegree(joint.rotation.y),
            MathRadianToDegree(joint.rotation.z) );

        for( size_t j = 0; j < joint.rotationKeys.Size(); j++ )
        {
            ms3d_keyframe_t& keyframe = joint.rotationKeys[j];

            keyframe.parameter = Vector3(
                MathRadianToDegree(keyframe.parameter.x),
                MathRadianToDegree(keyframe.parameter.y),
                MathRadianToDegree(keyframe.parameter.z) );
        }
    }
}

//-----------------------------------//

void Milkshape3D::buildAnimationMetadata()
{
    if( mainComment.Empty() )
    {
        AnimationMetadata data;
        
        data.start = 1;
        data.end = totalFrames;

        metadata.Push(data);

        return;
    }

    for( size_t i = 0; i < mainComment.Length(); i++ )
    {
        char& c = mainComment[i];

        if( c == 10 ) c = ' ';
        if( c == 13 ) c = '\n';
    }

    Vector<UTF8String> lines;
    StringSplit(mainComment, '\n', lines);

    for( size_t i = 0; i < lines.Size(); i++ )
    {
        const UTF8String& line = lines[i];

        if( line.Length() < 2 )
            continue;

        std::stringstream ss(line.CString());

        AnimationMetadata data;

        ss >> data.start;
        ss >> data.end;
        ss >> data.name;

        data.startTime = getAnimationStart(data);

        metadata.Push(data);
    }
}

//-----------------------------------//

void Milkshape3D::buildAnimations()
{
    assert( mesh->skeleton != nullptr );

    buildAnimationMetadata();

    for( size_t i = 0; i < metadata.Size(); i++ )
    {
        AnimationMetadata& data = metadata[i];
        
        Animation* animation = buildAnimation(data);
        mesh->animations.Push(animation);
    }

    // Builds the bind pose animation.
    AnimationMetadata bindPoseMetadata;
    
    bindPoseMetadata.start = 0;
    bindPoseMetadata.end = 0;
    bindPoseMetadata.name = "Bind";
    bindPoseMetadata.startTime = 0;

    mesh->bindPose = buildAnimation(bindPoseMetadata);
}

//-----------------------------------//

Animation* Milkshape3D::buildAnimation(AnimationMetadata& data)
{
    Animation* animation = AllocateThis(Animation);
    animation->setName(data.name);

    for( size_t i = 0; i < joints.Size(); i++ )
    {
        ms3d_joint_t& joint = joints[i];

        if( joint.positionKeys.Empty() )
            continue;

        Bone* bone = mesh->skeleton->findBone(joint.name).get();

        KeyFramesVector frames;
        buildKeyFrames( joint, data, frames );

        animation->setKeyFrames(bone, frames);
    }

    return animation;
}

//-----------------------------------//

void Milkshape3D::buildKeyFrames( const ms3d_joint_t& joint,
        const AnimationMetadata& data, KeyFramesVector& frames )
{
    assert( joint.positionKeys.Size() == joint.rotationKeys.Size() );

    for( size_t i = 0; i < joint.positionKeys.Size(); i++ )
    {
        const ms3d_keyframe_t& frame = joint.positionKeys[i];

        float time = frame.time*animationFPS + 0.5f;
        int numFrame = (int) std::floor(time);

        if( numFrame < data.start || numFrame > data.end )
            continue;

        KeyFrame key;

        key.time = (frame.time - data.startTime)*animationFPS;
        key.position = frame.parameter;
        key.rotation = (EulerAngles&) joint.rotationKeys[i].parameter;

        frames.Push(key);
    }
}

//-----------------------------------//

float Milkshape3D::getAnimationStart(const AnimationMetadata& data)
{
    float minTime = LimitsFloatMaximum;

    for( size_t i = 0; i < joints.Size(); i++ )
    {
        const ms3d_joint_t& joint = joints[i];

        for( size_t j = 0; j < joint.positionKeys.Size(); j++ )
        {
            const ms3d_keyframe_t& frame = joint.positionKeys[j];

            float time = frame.time*animationFPS + 0.5f;
            int numFrame = (int) std::floor(time);

            if( numFrame < data.start || numFrame > data.end )
                continue;

            minTime = std::min(minTime, frame.time);
        }
    }

    return minTime;
}

//-----------------------------------//

struct MilkshapeMeshVertex
{
    Vector3P position;
    Vector3P normal;
    Vector2 texCoord;
    float boneIndex;
};

static VertexElementP s_MilkshapeMeshElements[] =
{
    { VertexAttribute::Position, VertexDataType::Float, 3 },
    { VertexAttribute::Normal, VertexDataType::Float, 3 },
    { VertexAttribute::TexCoord0, VertexDataType::Float, 2 },
    { VertexAttribute::BoneIndex, VertexDataType::Float, 1 }
};

void Milkshape3D::buildGeometry()
{
    GeometryBuffer* gb = mesh->getGeometryBuffer().get();

    if( !gb )
    {
        gb = AllocateThis(GeometryBuffer);
        mesh->setGeometryBuffer(gb);
    }

    // Setup the vertex format for the mesh.
    static size_t numVertexElements = FLD_ARRAY_SIZE(s_MilkshapeMeshElements);
    gb->declarations.decls.Reserve(numVertexElements);

    for(size_t i = 0; i < numVertexElements; ++i )
        gb->declarations.add(s_MilkshapeMeshElements[i]);
    
    gb->declarations.calculateStrides();

    // Setup the mesh groups.
    mesh->groups.Resize( groups.Size() );

    for( size_t i = 0; i < groups.Size(); i++ )
    {
        const ms3d_group_t& group = groups[i];

        if( group.flags & HIDDEN )
            continue;

        size_t numIndices = group.triangleIndices.Size();

        // In case this group doesn't have geometry, skip processing.
        if( numIndices == 0 ) continue;

        MeshGroup& meshGroup = mesh->groups[i];
        meshGroup.indices.Reserve( numIndices*3 );
        meshGroup.material = buildMaterial(group);

        gb->data.Reserve(numIndices * 3);

        int numVertex = 0;

        // Let's process all the triangles of this group.
        for( size_t j = 0; j < numIndices; j++ )
        {
            const uint16& triIndex = group.triangleIndices[j];
            const ms3d_triangle_t& t = *triangles[triIndex];

            for( uint8 e = 0; e < 3; e++ )
            {
                const uint16& vertexIndex = t.vertexIndices[e];
                const ms3d_vertex_t& vertex = *vertices[vertexIndex];

                MilkshapeMeshVertex meshVertex = {
                    vertex.position,
                    t.vertexNormals[e],
                    Vector2(t.s[e], t.t[e]),
                    vertex.boneIndex
                };

                gb->add((uint8*)&meshVertex, sizeof(meshVertex));
                meshGroup.indices.Push( numVertex++ );
            }
        }
    }
}

//-----------------------------------//
    
MeshMaterial Milkshape3D::buildMaterial(const ms3d_group_t& group)
{
    MeshMaterial material;
    material.alpha = false;

    if( !hasMaterial(group) )
        return material;

    const ms3d_material_t& mt = *materials[group.materialIndex];

    material.name = mt.name;

    if( strlen(mt.texture) > 0 )
    {
        const String& path = PathNormalize(mt.texture);
        material.texture = path;
    }

    if( mt.mode & HASALPHA )
    {
        material.alpha = true;
    }

    return material;
}

//-----------------------------------//

bool Milkshape3D::hasMaterial( const ms3d_group_t& g )
{
    // Let's check if we have a valid material in the structure.
    return (g.materialIndex != -1) 
        && (g.materialIndex >= 0) 
        && (g.materialIndex < signed(materials.Size()));
}

//-----------------------------------//

/**
 * We do this hack to save memory. Since we already have the data in memory
 * we can just reinterpret it as the correct structure. triton saves the day!
 * I know this is horrible, but it's working fine, so don't touch it. :)
 */

#define FILEBUF_INDEX(type)                                             \
    *reinterpret_cast<type*>(&filebuf[index]); index+=sizeof(type);

#define FILEBUF_READ(type, ptr)                                         \
    { ptr = reinterpret_cast<type*>(&filebuf[index]); index+=sizeof(type); }

#define MEMCPY_SKIP_INDEX(a,b)                                          \
    memcpy(&a,&filebuf[index],b); index+=b;

//-----------------------------------//

bool Milkshape3D::readHeader()
{
    ms3d_header_t& header = FILEBUF_INDEX(ms3d_header_t);

    if(strncmp(header.id, "MS3D000000", 10) != 0) 
        return false;

    if(header.version != 4)
        return false;

    return true;
}

//-----------------------------------//

void Milkshape3D::readVertices()
{
    uint16& numVertices = FILEBUF_INDEX(uint16);
    vertices.Resize(numVertices);

    for (int i = 0; i < numVertices; i++)
    {
        FILEBUF_READ(ms3d_vertex_t, vertices[i]);
    }
}

//-----------------------------------//

void Milkshape3D::readTriangles()
{
    uint16& numTriangles = FILEBUF_INDEX(uint16);
    triangles.Resize(numTriangles);
    
    for (int i = 0; i < numTriangles; i++)
    {
        FILEBUF_READ(ms3d_triangle_t, triangles[i]);
    }
}

//-----------------------------------//

void Milkshape3D::readGroups()
{
    uint16& numGroups = FILEBUF_INDEX(uint16);
    groups.Resize(numGroups);
    
    for (int i = 0; i < numGroups; i++)
    {
        MEMCPY_SKIP_INDEX(groups[i].flags, sizeof(uint8));
        MEMCPY_SKIP_INDEX(groups[i].name, sizeof(char)*32);

        uint16 numGroupTriangles;
        MEMCPY_SKIP_INDEX(numGroupTriangles, sizeof(uint16));
        groups[i].triangleIndices.Resize(numGroupTriangles);

        if (numGroupTriangles > 0)
        {
            MEMCPY_SKIP_INDEX(groups[i].triangleIndices[0],
             sizeof(uint16) * numGroupTriangles);
        }

        MEMCPY_SKIP_INDEX(groups[i].materialIndex, sizeof(uint8));
    }
}

//-----------------------------------//

void Milkshape3D::readMaterials()
{
    uint16& numMaterials = FILEBUF_INDEX(uint16);
    materials.Resize(numMaterials);
    
    for (int i = 0; i < numMaterials; i++)
    {
        FILEBUF_READ(ms3d_material_t, materials[i]);

        ms3d_material_t& mat = *materials[i];
        float transparency = mat.transparency;

        // Set alpha of the material colors.
        mat.ambient.a = transparency;
        mat.diffuse.a = transparency;
        mat.specular.a = transparency;
        mat.emissive.a = transparency;
    }
}

//-----------------------------------//

void Milkshape3D::readAnimation()
{
    animationFPS = FILEBUF_INDEX(float);
    
    if (animationFPS < 1.0f)
        animationFPS = 1.0f;
    
    float& m_currentTime = FILEBUF_INDEX(float);
    totalFrames = (int) FILEBUF_INDEX(float);
}

//-----------------------------------//

void Milkshape3D::readJoints()
{
    uint16& numJoints = FILEBUF_INDEX(uint16);
    joints.Resize(numJoints);
    
    for (uint32 i = 0; i < numJoints; i++)
    {
        MEMCPY_SKIP_INDEX(joints[i].flags, sizeof(byte));
        MEMCPY_SKIP_INDEX(joints[i].name, sizeof(char)*32);
        MEMCPY_SKIP_INDEX(joints[i].parentName, sizeof(char)*32);
        MEMCPY_SKIP_INDEX(joints[i].rotation, sizeof(EulerAngles));
        MEMCPY_SKIP_INDEX(joints[i].position, sizeof(Vector3));
        joints[i].indexParent = -1;

        uint16& numKeyFramesRot = FILEBUF_INDEX(uint16);
        joints[i].rotationKeys.Resize(numKeyFramesRot);

        uint16& numKeyFramesPos = FILEBUF_INDEX(uint16);
        joints[i].positionKeys.Resize(numKeyFramesPos);

        // the frame time is in seconds, so multiply it by the animation fps,
        // to get the frames rotation channel
        for (uint32 j = 0; j < numKeyFramesRot; j++)
        {
            joints[i].rotationKeys[j].time = FILEBUF_INDEX(float);
            joints[i].rotationKeys[j].parameter = FILEBUF_INDEX(Vector3);
        }

        // translation channel
        for (uint32 j = 0; j < numKeyFramesPos; j++)
        {
            joints[i].positionKeys[j].time = FILEBUF_INDEX(float);
            joints[i].positionKeys[j].parameter = FILEBUF_INDEX(Vector3);
        }
    }
}

//-----------------------------------//

void Milkshape3D::readComments()
{
    if( index == filebuf.Size() )
        return;

    int subVersion = FILEBUF_INDEX(int);
    
    if( subVersion != 1 )
    {
        LogWarn( "Unknown subversion comment chunk: '%d'", subVersion );
        return;
    }

    uint32 numComments = 0;
    size_t commentSize = 0;

    // group comments
    numComments = FILEBUF_INDEX(int);
    
    for( uint32 i = 0; i < numComments; ++i )
    {
        int& groupIndex = FILEBUF_INDEX(int);
        commentSize = FILEBUF_INDEX(size_t);
        //groupComments.resize(commentSize);
        
        //if(commentSize > 0)
            //FILEBUF_READ(&comment[0], sizeof(char), commentSize, fp);
        index += commentSize;
        
        //if (index >= 0 && index < (int) groups.size())
        //  groups[index].comment = comment;
    }

    // material comments
    numComments = FILEBUF_INDEX(int);

    for( uint32 i = 0; i < numComments; ++i )
    {
        int& matIndex = FILEBUF_INDEX(int);
        commentSize = FILEBUF_INDEX(size_t);
        //jointComments.resize(commentSize);
        
        //if(commentSize > 0)
            //FILEBUF_READ(&comment[0], sizeof(char), commentSize, fp);
        index += commentSize;
        
        //if (index >= 0 && index < (int) materials.size())
        //  materials[index].comment = comment;
    }

    // joint comments
    numComments = FILEBUF_INDEX(int);

    for( uint32 i = 0; i < numComments; ++i )
    {
        int& jointIndex = FILEBUF_INDEX(int);
        commentSize = FILEBUF_INDEX(size_t);
        //groupComments.resize(commentSize);
        
        //if(commentSize > 0)
            //FILEBUF_READ(&comment[0], sizeof(char), commentSize, fp);
        index += commentSize;
        
        //if (index >= 0 && index < (int) joints.size())
            //joints[index].comment = comment;
    }

    // model comments
    numComments = FILEBUF_INDEX(int);

    if (numComments == 1)
    {
        commentSize = FILEBUF_INDEX(size_t);

        char* temp = new char[commentSize+1];
        
        if (commentSize > 0)
            MEMCPY_SKIP_INDEX(temp[0], commentSize);

        temp[commentSize] = '\0';

        mainComment = temp;

        delete[] temp;
    }
}

//-----------------------------------//

NAMESPACE_RESOURCES_END