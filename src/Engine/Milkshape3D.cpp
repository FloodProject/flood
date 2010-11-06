/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

// based on official Milkshape3D viewer source

#include "vapor/PCH.h"
#include "vapor/resources/Milkshape3D.h"
#include "vapor/render/Renderable.h"
#include "vapor/animation/Skeleton.h"
#include "vapor/animation/Animation.h"
#include "vapor/math/Math.h"
#include "vapor/Utilities.h"
#include "Milkshape3D_Specs.h"

namespace vapor {

//-----------------------------------//

Milkshape3D::Milkshape3D()
	: index(0)
{ }

//-----------------------------------//

bool Milkshape3D::load(const Stream& stream)
{
	filebuf = stream.read();

	if( filebuf.empty() ) 
		return false;

	if( !parse() )
	{
		filebuf.clear();
		return false;
	}
	
	filebuf.clear();
	return true;
}

//-----------------------------------//

bool Milkshape3D::parse()
{
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
	animated = !joints.empty();

	buildBounds();

	if( isAnimated() )
	{
		buildSkeleton();
		buildAnimations();
		setupInitialVertices();
	}
}

//-----------------------------------//

void Milkshape3D::build()
{
	if( built )
		return;

	buildGeometry();

	built = true;
}

//-----------------------------------//

void Milkshape3D::buildSkeleton()
{
	setupJointsHierarchy();
	setupJointRotations();
	setupJointMatrices();

	skeleton = new Skeleton();
	
	uint index = 0;

	foreach( ms3d_joint_t& joint, joints )
	{
		BonePtr bone = new Bone();

		bone->name = joint.name;
		bone->index = index++;
		bone->parentIndex = joint.parentIndex;
		bone->position = joint.position;
		bone->rotation = joint.rotation;
		bone->relativeMatrix = joint.relativeMatrix;
		bone->absoluteMatrix = joint.absoluteMatrix;

		skeleton->addBone(bone);
	}
}

//-----------------------------------//

void Milkshape3D::buildAnimationMetadata()
{
	if( mainComment.empty() )
	{
		AnimationMetadata data;
		
		data.start = 1;
		data.end = totalFrames;

		metadata.push_back(data);

		return;
	}

	foreach( char& c, mainComment )
	{
		if( c == 10 )
			c = ' ';
		if( c == 13 )
			c = '\n';
	}

	std::vector<std::string> lines = String::split(mainComment, '\n');
    
	foreach( const std::string& line, lines )
	{
		if( line.size() < 2 )
			continue;

		std::stringstream ss(line);

		AnimationMetadata data;

		ss >> data.start;
		ss >> data.end;
		ss >> data.name;

		data.startTime = getAnimationStart(data);

		metadata.push_back(data);
	}
}

//-----------------------------------//

float Milkshape3D::getAnimationStart(const AnimationMetadata& data)
{
	float time = Limits::FloatMaximum;

	foreach( const ms3d_joint_t& joint, joints )
	{
		foreach( const ms3d_keyframe_t& frame, joint.positionKeys )
		{
			int numFrame = std::floor(frame.time*animationFPS + 0.5);

			if( numFrame < data.start || numFrame > data.end )
				continue;

			time = std::min(time, frame.time);
		}
	}

	return time;
}

//-----------------------------------//

void Milkshape3D::buildAnimations()
{
	assert( skeleton != nullptr );

	buildAnimationMetadata();

	foreach( AnimationMetadata& data, metadata )
	{
		AnimationPtr animation = buildAnimation(data);
		animations.push_back(animation);
	}

	// Builds the bind pose animation.
	AnimationMetadata bindPoseMetadata;
	
	bindPoseMetadata.start = 0;
	bindPoseMetadata.end = 0;
	bindPoseMetadata.name = "Bind";
	bindPoseMetadata.startTime = 0;

	bindPose = buildAnimation(bindPoseMetadata);
}

//-----------------------------------//

AnimationPtr Milkshape3D::buildAnimation(AnimationMetadata& data)
{
	AnimationPtr animation = new Animation();
	animation->setName(data.name);

	foreach( ms3d_joint_t& joint, joints )
	{
		if( joint.positionKeys.empty() )
			continue;

		const BonePtr& bone = skeleton->findBone(joint.name);

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
	assert( joint.positionKeys.size() == joint.rotationKeys.size() );

	uint i = 0;

	foreach( const ms3d_keyframe_t& frame, joint.positionKeys )
	{
		int numFrame = std::floor(frame.time*animationFPS + 0.5);

		if( numFrame < data.start || numFrame > data.end )
		{
			i++;
			continue;
		}

		KeyFrame key;

		key.time = (frame.time - data.startTime)*animationFPS;
		key.position = frame.parameter;
		key.rotation = (EulerAngles&) joint.rotationKeys[i++].parameter;

		frames.push_back(key);
	}
}

//-----------------------------------//

void Milkshape3D::buildGeometry()
{
	foreach( const ms3d_group_t& group, groups )
	{
		if( group.flags & HIDDEN )
			continue;

		// In case this group doesn't have geometry, skip processing.
		if( group.triangleIndices.empty() )
			continue;

		// Vertex data
		std::vector<Vector3> pos;
		pos.reserve( group.triangleIndices.size()*3 );

		// Normal data
		std::vector< Vector3 > normals;
		normals.reserve( group.triangleIndices.size()*3 );
		
		// Texture coords data
		std::vector<Vector3> texCoords;
		texCoords.reserve( group.triangleIndices.size()*3 );

		// Bones data
		std::vector<float> bones;
		bones.reserve( group.triangleIndices.size()*3 );

		// Let's process all the triangles of this group.
		foreach( const ushort& t_ind, group.triangleIndices )
		{
			const ms3d_triangle_t& t = *triangles[t_ind];

			foreach( const ushort& v_ind, t.vertexIndices ) 
			{
				const ms3d_vertex_t& v = *vertices[v_ind];
			
				pos.push_back( v.position );
				bones.push_back( v.boneIndex );
			}

			for( uint i = 0; i < 3; i++ )
			{
				Vector3 normal( t.vertexNormals[i] );
				normals.push_back( normal );

				Vector2 texCoord( t.s[i], t.t[i] );
				texCoords.push_back( texCoord );
			}
		}

		// Get a shiny material for the group.
		MaterialPtr mat = buildMaterial(group);

		// Vertex buffer.
		VertexBufferPtr vb( new VertexBuffer() );

		if( isAnimated() )
			vb->set( VertexAttribute::FogCoord, bones );

		vb->set( VertexAttribute::Position, pos );
		vb->set( VertexAttribute::Normal, normals );
		vb->set( VertexAttribute::TexCoord0, texCoords );

		// Renderable.
		RenderablePtr rend( new Renderable() );
		rend->setPrimitiveType( PolygonType::Triangles );
		rend->setVertexBuffer( vb );
		rend->setMaterial( mat );
		renderables.push_back( rend );

		#pragma TODO("Use index buffers when building mesh geometry")
		
		//std::vector< ushort > vb_i;
		//foreach( const ms3d_triangle_t& t, triangles )
		//{	
		//	vb_i.push_back( t.vertexIndices[0] );
		//	vb_i.push_back( t.vertexIndices[1] );
		//	vb_i.push_back( t.vertexIndices[2] );
		//}

		//IndexBufferPtr ib( new IndexBuffer() );
		//ib->set( vb_i );
		//rend->setIndexBuffer( ib );
	}
}

//-----------------------------------//
	
MaterialPtr Milkshape3D::buildMaterial(const ms3d_group_t& group)
{
	if( !hasMaterial(group) )
		return nullptr;

	const ms3d_material_t& mt = *materials[group.materialIndex];

	MaterialPtr mat( new Material(mt.name) );

	if( strlen(mt.texture) > 0 )
	{
		mat->setProgram( "Tex_Toon" );

		std::string texturePath = String::split( mt.texture, '/' ).back();
		mat->setTexture( 0, texturePath );
	}

	if( mt.mode & HASALPHA )
	{
		#pragma TODO("Use alpha testing when alpha values are fully transparent.")
		mat->setAlphaTest(true);
		//mat->setBlending( BlendSource::SourceAlpha, BlendDestination::InverseSourceAlpha );
	}

	if( isAnimated() )
		mat->setProgram( "Tex_Toon_Skin" );

	return mat;
}

//-----------------------------------//

void Milkshape3D::buildBounds()
{
	boundingVolume.reset();

	// Update the bounding box to accomodate new geometry.
	foreach( const ms3d_vertex_t* vert, vertices )
		boundingVolume.add( vert->position );

	if( boundingVolume.isInfinite() )
		boundingVolume.setZero();
}

//-----------------------------------//

int Milkshape3D::findJoint(const char* name)
{
	if( strlen(name) == 0 )
		return -1;

	for( uint i = 0; i < joints.size(); i++ )
	{
		if( strcmp(joints[i].name, name) == 0 )
			return i;
	}

	return -1;
}

//-----------------------------------//

void Milkshape3D::setupJointsHierarchy()
{
	foreach (ms3d_joint_t& joint, joints )
	{
		joint.parentIndex = findJoint(joint.parentName);
	}
}

//-----------------------------------//

void Milkshape3D::setupJointMatrices()
{
	foreach( ms3d_joint_t& joint, joints )
	{
		joint.relativeMatrix =
			Matrix4x3::createRotation(joint.rotation) *
			Matrix4x3::createTranslation(joint.position);

		if( joint.parentIndex != -1 )
		{
			const ms3d_joint_t& parent = joints[joint.parentIndex];
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
	foreach( ms3d_joint_t& joint, joints )
	{
		joint.rotation = EulerAngles(
			Math::radianToDegree(joint.rotation.x),
			Math::radianToDegree(joint.rotation.y),
			Math::radianToDegree(joint.rotation.z) );

		foreach( ms3d_keyframe_t& keyframe, joint.rotationKeys )
		{
			keyframe.parameter = Vector3(
				Math::radianToDegree(keyframe.parameter.x),
				Math::radianToDegree(keyframe.parameter.y),
				Math::radianToDegree(keyframe.parameter.z) );
		}
	}
}

//-----------------------------------//

void Milkshape3D::setupInitialVertices()
{
	foreach( ms3d_vertex_t* vertex, vertices )
	{
		if( vertex->boneIndex == -1 )
			continue;
		
		ms3d_joint_t& joint = joints[vertex->boneIndex];
		
		Matrix4x3 invJoint = joint.absoluteMatrix.inverse();
		vertex->position = invJoint*vertex->position;
	}
}

//-----------------------------------//

bool Milkshape3D::hasMaterial( const ms3d_group_t& g )
{
	// Let's check if we have a valid material in the structure.
	return (g.materialIndex != -1) 
		&& (g.materialIndex >= 0) 
		&& (g.materialIndex < signed(materials.size()));
}

//-----------------------------------//

/**
 * We do this hack to save memory. Since we already have the data in memory
 * we can just reinterpret it as the correct structure. triton saves the day!
 * I know this is horrible, but it's working fine, so don't touch it. :)
 */

#define FILEBUF_INDEX(type)												\
	*reinterpret_cast<type*>(&filebuf[index]); index+=sizeof(type);

#define FILEBUF_READ(type, ptr)											\
	{ ptr = reinterpret_cast<type*>(&filebuf[index]); index+=sizeof(type); }

#define MEMCPY_SKIP_INDEX(a,b)											\
	memcpy(&a,&filebuf[index],b); index+=b;

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
	ushort& numVertices = FILEBUF_INDEX(ushort);
	vertices.resize(numVertices);

	for (int i = 0; i < numVertices; i++)
	{
		FILEBUF_READ(ms3d_vertex_t, vertices[i]);
	}
}

//-----------------------------------//

void Milkshape3D::readTriangles()
{
	ushort& numTriangles = FILEBUF_INDEX(ushort);
	triangles.resize(numTriangles);
	
	for (int i = 0; i < numTriangles; i++)
	{
		FILEBUF_READ(ms3d_triangle_t, triangles[i]);
	}
}

//-----------------------------------//

void Milkshape3D::readGroups()
{
	ushort& numGroups = FILEBUF_INDEX(ushort);
	groups.resize(numGroups);
	
	for (int i = 0; i < numGroups; i++)
	{
		MEMCPY_SKIP_INDEX(groups[i].flags, sizeof(byte));
		MEMCPY_SKIP_INDEX(groups[i].name, sizeof(char)*32);

		ushort numGroupTriangles;
		MEMCPY_SKIP_INDEX(numGroupTriangles, sizeof(ushort));
		groups[i].triangleIndices.resize(numGroupTriangles);

		if (numGroupTriangles > 0)
		{
			MEMCPY_SKIP_INDEX(groups[i].triangleIndices[0],
				sizeof(ushort)*numGroupTriangles);
		}

		MEMCPY_SKIP_INDEX(groups[i].materialIndex, sizeof(byte));
	}
}

//-----------------------------------//

void Milkshape3D::readMaterials()
{
	ushort& numMaterials = FILEBUF_INDEX(ushort);
	materials.resize(numMaterials);
	
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
	totalFrames = FILEBUF_INDEX(float);
}

//-----------------------------------//

void Milkshape3D::readJoints()
{
	ushort& numJoints = FILEBUF_INDEX(ushort);
	joints.resize(numJoints);
	
	for (uint i = 0; i < numJoints; i++)
	{
		MEMCPY_SKIP_INDEX(joints[i].flags, sizeof(byte));
		MEMCPY_SKIP_INDEX(joints[i].name, sizeof(char)*32);
		MEMCPY_SKIP_INDEX(joints[i].parentName, sizeof(char)*32);
		MEMCPY_SKIP_INDEX(joints[i].rotation, sizeof(EulerAngles));
		MEMCPY_SKIP_INDEX(joints[i].position, sizeof(Vector3));
		joints[i].parentIndex = -1;

		ushort& numKeyFramesRot = FILEBUF_INDEX(ushort);
		joints[i].rotationKeys.resize(numKeyFramesRot);

		ushort& numKeyFramesPos = FILEBUF_INDEX(ushort);
		joints[i].positionKeys.resize(numKeyFramesPos);

		// the frame time is in seconds, so multiply it by the animation fps,
		// to get the frames rotation channel
		for (uint j = 0; j < numKeyFramesRot; j++)
		{
			joints[i].rotationKeys[j].time = FILEBUF_INDEX(float);
			joints[i].rotationKeys[j].parameter = FILEBUF_INDEX(Vector3);
		}

		// translation channel
		for (uint j = 0; j < numKeyFramesPos; j++)
		{
			joints[i].positionKeys[j].time = FILEBUF_INDEX(float);
			joints[i].positionKeys[j].parameter = FILEBUF_INDEX(Vector3);
		}
	}
}

//-----------------------------------//

void Milkshape3D::readComments()
{
	if( index == filebuf.size() )
		return;

	int subVersion = FILEBUF_INDEX(int);
	
	if( subVersion != 1 )
	{
		Log::warn( "Unknown subversion comment chunk: '%d'", subVersion );
		return;
	}

	uint numComments = 0;
	size_t commentSize = 0;

	// group comments
	numComments = FILEBUF_INDEX(int);
	
	for( uint i = 0; i < numComments; ++i )
	{
		int& groupIndex = FILEBUF_INDEX(int);
		commentSize = FILEBUF_INDEX(size_t);
		//groupComments.resize(commentSize);
		
		//if(commentSize > 0)
			//FILEBUF_READ(&comment[0], sizeof(char), commentSize, fp);
		index += commentSize;
		
		//if (index >= 0 && index < (int) groups.size())
		//	groups[index].comment = comment;
	}

	// material comments
	numComments = FILEBUF_INDEX(int);

	for( uint i = 0; i < numComments; ++i )
	{
		int& matIndex = FILEBUF_INDEX(int);
		commentSize = FILEBUF_INDEX(size_t);
		//jointComments.resize(commentSize);
		
		//if(commentSize > 0)
			//FILEBUF_READ(&comment[0], sizeof(char), commentSize, fp);
		index += commentSize;
		
		//if (index >= 0 && index < (int) materials.size())
		//	materials[index].comment = comment;
	}

	// joint comments
	numComments = FILEBUF_INDEX(int);

	for( uint i = 0; i < numComments; ++i )
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

} // end namespace