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
#include "Milkshape3D_Specs.h"

namespace vapor {

//-----------------------------------//

Milkshape3D::Milkshape3D()
	: index(0)
{ }

//-----------------------------------//

bool Milkshape3D::load(const File& file)
{
	filebuf = file.read();

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
	//readComments();

	return true;
}

//-----------------------------------//

void Milkshape3D::build()
{
	if(built)
		return;

	animated = !joints.empty();

	if( isAnimated() )
	{
		buildSkeleton();
		buildAnimation();
		
		//setupTangents();
		setupInitialVertices();
	}

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

void Milkshape3D::buildAnimation()
{
	assert( skeleton != nullptr );

	uint numAnimations = 1/*joints[0].positionKeys.size()*/;
	
	for( uint i = 0; i < numAnimations; i++ )
	{
		AnimationPtr animation = new Animation();

		uint startFrame = 0;
		uint endFrame = joints[0].positionKeys.size()-1;

		foreach( ms3d_joint_t& joint, joints )
		{
			if( joint.positionKeys.empty() )
				continue;

			const BonePtr& bone = skeleton->findBone(joint.name);

			KeyFramesVector frames;

			for( uint j = 0; j < joint.positionKeys.size(); j++ )
			{
				KeyFrame frame;

				frame.time = joint.positionKeys[j].time*animationFPS;
				frame.position = joint.positionKeys[j].parameter;
				frame.rotation = (EulerAngles&) joint.rotationKeys[j].parameter;

				frames.push_back(frame);
			}

			animation->setKeyFrames(bone, frames);
		}

		animations.push_back(animation);
	}
}

//-----------------------------------//

void Milkshape3D::buildGeometry()
{
	foreach( const ms3d_group_t& group, groups )
	{
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
		std::vector< Vector3 > texCoords;
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
		mat->setTexture( 0, mt.texture );
	}

	if( mt.mode & HASALPHA )
	{
		#pragma TODO("Use alpha testing when alpha values are fully transparent.")

		mat->setBlending( BlendingSource::SourceAlpha,
			BlendingDestination::InverseSourceAlpha );
	}

	if( isAnimated() )
	{
		mat->setProgram( "Tex_Toon_Skin" );
	}

	return mat;
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
			ms3d_joint_t& parent = joints[joint.parentIndex];
			joint.absoluteMatrix = parent.absoluteMatrix * joint.relativeMatrix;
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
		
		Matrix4x3 invJoint = inverse(joint.absoluteMatrix);
		vertex->position = vertex->position * invJoint;
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

	if (strncmp(header.id, "MS3D000000", 10) != 0) 
		return false;

	if (header.version != 4)
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
//	// comments
//	long filePos = ftell(fp);
//	if (filePos < fileSize)
//	{
//		int subVersion = 0;
//		//FILEBUF_READ(&subVersion, sizeof(int), 1, fp);
//		if (subVersion == 1)
//		{
//			int numComments = 0;
//			size_t commentSize = 0;
//
//			// group comments
//			//FILEBUF_READ(&numComments, sizeof(int), 1, fp); 
//			for (i = 0; i < numComments; i++)
//			{
//				int index;
//				//FILEBUF_READ(&index, sizeof(int), 1, fp);
//				std::vector<char> comment;
//				//FILEBUF_READ(&commentSize, sizeof(size_t), 1, fp);
//				comment.resize(commentSize);
//				if (commentSize > 0)
//					//FILEBUF_READ(&comment[0], sizeof(char), commentSize, fp);
//				if (index >= 0 && index < (int) groups.size())
//					groups[index].comment = comment;
//			}
//
//			// material comments
//			//FILEBUF_READ(&numComments, sizeof(int), 1, fp); 
//			for (i = 0; i < numComments; i++)
//			{
//				int index;
//				//FILEBUF_READ(&index, sizeof(int), 1, fp);
//				std::vector<char> comment;
//				//FILEBUF_READ(&commentSize, sizeof(size_t), 1, fp);
//				comment.resize(commentSize);
//				if (commentSize > 0)
//					//FILEBUF_READ(&comment[0], sizeof(char), commentSize, fp);
//				if (index >= 0 && index < (int) materials.size())
//					materials[index].comment = comment;
//			}
//
//			// joint comments
//			//FILEBUF_READ(&numComments, sizeof(int), 1, fp); 
//			for (i = 0; i < numComments; i++)
//			{
//				int index;
//				//FILEBUF_READ(&index, sizeof(int), 1, fp);
//				std::vector<char> comment;
//				//FILEBUF_READ(&commentSize, sizeof(size_t), 1, fp);
//				comment.resize(commentSize);
//				if (commentSize > 0)
//					//FILEBUF_READ(&comment[0], sizeof(char), commentSize, fp);
//				if (index >= 0 && index < (int) joints.size())
//					joints[index].comment = comment;
//			}
//
//			// model comments
//			//FILEBUF_READ(&numComments, sizeof(int), 1, fp);
//			if (numComments == 1)
//			{
//				std::vector<char> comment;
//				//FILEBUF_READ(&commentSize, sizeof(size_t), 1, fp);
//				comment.resize(commentSize);
//				if (commentSize > 0)
//					//FILEBUF_READ(&comment[0], sizeof(char), commentSize, fp);
//				m_comment = comment;
//			}
//		}
//		else
//		{
//			// "Unknown subversion for comments %d\n", subVersion);
//		}
//	}
}
//
//
//void Milkshape3D::read_extra()
//{
//	// vertex extra
//	filePos = ftell(fp);
//	if (filePos < fileSize)
//	{
//		int subVersion = 0;
//		//FILEBUF_READ(&subVersion, sizeof(int), 1, fp);
//		if (subVersion == 2)
//		{
//			for (int i = 0; i < numVertices; i++)
//			{
//				//FILEBUF_READ(&vertices[i].boneIds[0], sizeof(char), 3, fp);
//				//FILEBUF_READ(&vertices[i].weights[0], sizeof(byte), 3, fp);
//				//FILEBUF_READ(&vertices[i].extra, sizeof(unsigned int), 1, fp);
//			}
//		}
//		else if (subVersion == 1)
//		{
//			for (int i = 0; i < numVertices; i++)
//			{
//				//FILEBUF_READ(&vertices[i].boneIds[0], sizeof(char), 3, fp);
//				//FILEBUF_READ(&vertices[i].weights[0], sizeof(byte), 3, fp);
//			}
//		}
//		else
//		{
//			// "Unknown subversion for vertex extra %d\n", subVersion);
//		}
//	}
//
//	// joint extra
//	filePos = ftell(fp);
//	if (filePos < fileSize)
//	{
//		int subVersion = 0;
//		//FILEBUF_READ(&subVersion, sizeof(int), 1, fp);
//		if (subVersion == 1)
//		{
//			for (int i = 0; i < numJoints; i++)
//			{
//				//FILEBUF_READ(&joints[i].color, sizeof(float), 3, fp);
//			}
//		}
//		else
//		{
//			// "Unknown subversion for joint extra %d\n", subVersion);
//		}
//	}
//
//	// model extra
//	filePos = ftell(fp);
//	if (filePos < fileSize)
//	{
//		int subVersion = 0;
//		//FILEBUF_READ(&subVersion, sizeof(int), 1, fp);
//		if (subVersion == 1)
//		{
//			//FILEBUF_READ(&jointsize, sizeof(float), 1, fp);
//			//FILEBUF_READ(&m_transparencyMode, sizeof(int), 1, fp);
//			//FILEBUF_READ(&m_alphaRef, sizeof(float), 1, fp);
//		}
//		else
//		{
//			//"Unknown subversion for model extra %d\n", subVersion);
//		}
//	}
//}

//void msModel::SetupTangents()
//{
//	for (size_t j = 0; j < joints.size(); j++)
//	{
//		ms3d_joint_t *joint = &joints[j];
//		int numPositionKeys = (int) joint->positionKeys.size();
//		joint->tangents.resize(numPositionKeys);
//
//		// clear all tangents (zero derivatives)
//		for (int k = 0; k < numPositionKeys; k++)
//		{
//			joint->tangents[k].tangentIn[0] = 0.0f;
//			joint->tangents[k].tangentIn[1] = 0.0f;
//			joint->tangents[k].tangentIn[2] = 0.0f;
//			joint->tangents[k].tangentOut[0] = 0.0f;
//			joint->tangents[k].tangentOut[1] = 0.0f;
//			joint->tangents[k].tangentOut[2] = 0.0f;
//		}
//
//		// if there are more than 2 keys, we can calculate tangents, otherwise we use zero derivatives
//		if (numPositionKeys > 2)
//		{
//			for (int k = 0; k < numPositionKeys; k++)
//			{
//				// make the curve tangents looped
//				int k0 = k - 1;
//				if (k0 < 0)
//					k0 = numPositionKeys - 1;
//				int k1 = k;
//				int k2 = k + 1;
//				if (k2 >= numPositionKeys)
//					k2 = 0;
//
//				// calculate the tangent, which is the vector from key[k - 1] to key[k + 1]
//				float tangent[3];
//				tangent[0] = (joint->positionKeys[k2].key[0] - joint->positionKeys[k0].key[0]);
//				tangent[1] = (joint->positionKeys[k2].key[1] - joint->positionKeys[k0].key[1]);
//				tangent[2] = (joint->positionKeys[k2].key[2] - joint->positionKeys[k0].key[2]);
//
//				// weight the incoming and outgoing tangent by their time to avoid changes in speed, if the keys are not within the same interval
//				float dt1 = joint->positionKeys[k1].time - joint->positionKeys[k0].time;
//				float dt2 = joint->positionKeys[k2].time - joint->positionKeys[k1].time;
//				float dt = dt1 + dt2;
//				joint->tangents[k1].tangentIn[0] = tangent[0] * dt1 / dt;
//				joint->tangents[k1].tangentIn[1] = tangent[1] * dt1 / dt;
//				joint->tangents[k1].tangentIn[2] = tangent[2] * dt1 / dt;
//
//				joint->tangents[k1].tangentOut[0] = tangent[0] * dt2 / dt;
//				joint->tangents[k1].tangentOut[1] = tangent[1] * dt2 / dt;
//				joint->tangents[k1].tangentOut[2] = tangent[2] * dt2 / dt;
//			}
//		}
//	}
//}
//
//void msModel::SetFrame(float frame)
//{
//	if (frame < 0.0f)
//	{
//		for (size_t i = 0; i < joints.size(); i++)
//		{
//			ms3d_joint_t *joint = &joints[i];
//			memcpy(joint->matLocal, joint->matLocalSkeleton, sizeof(joint->matLocal));
//			memcpy(joint->matGlobal, joint->matGlobalSkeleton, sizeof(joint->matGlobal));
//		}
//	}
//	else
//	{
//		for (size_t i = 0; i < joints.size(); i++)
//		{
//			EvaluateJoint(i, frame);
//		}
//	}
//
//	m_currentTime = frame;
//}
//
//void msModel::EvaluateJoint(int index, float frame)
//{
//	ms3d_joint_t *joint = &joints[index];
//
//	//
//	// calculate joint animation matrix, this matrix will animate matLocalSkeleton
//	//
//	vec3_t pos = { 0.0f, 0.0f, 0.0f };
//	int numPositionKeys = (int) joint->positionKeys.size();
//	if (numPositionKeys > 0)
//	{
//		int i1 = -1;
//		int i2 = -1;
//
//		// find the two keys, where "frame" is in between for the position channel
//		for (int i = 0; i < (numPositionKeys - 1); i++)
//		{
//			if (frame >= joint->positionKeys[i].time && frame < joint->positionKeys[i + 1].time)
//			{
//				i1 = i;
//				i2 = i + 1;
//				break;
//			}
//		}
//
//		// if there are no such keys
//		if (i1 == -1 || i2 == -1)
//		{
//			// either take the first
//			if (frame < joint->positionKeys[0].time)
//			{
//				pos[0] = joint->positionKeys[0].key[0];
//				pos[1] = joint->positionKeys[0].key[1];
//				pos[2] = joint->positionKeys[0].key[2];
//			}
//
//			// or the last key
//			else if (frame >= joint->positionKeys[numPositionKeys - 1].time)
//			{
//				pos[0] = joint->positionKeys[numPositionKeys - 1].key[0];
//				pos[1] = joint->positionKeys[numPositionKeys - 1].key[1];
//				pos[2] = joint->positionKeys[numPositionKeys - 1].key[2];
//			}
//		}
//
//		// there are such keys, so interpolate using hermite interpolation
//		else
//		{
//			ms3d_keyframe_t *p0 = &joint->positionKeys[i1];
//			ms3d_keyframe_t *p1 = &joint->positionKeys[i2];
//			ms3d_tangent_t *m0 = &joint->tangents[i1];
//			ms3d_tangent_t *m1 = &joint->tangents[i2];
//
//			// normalize the time between the keys into [0..1]
//			float t = (frame - joint->positionKeys[i1].time) / (joint->positionKeys[i2].time - joint->positionKeys[i1].time);
//			float t2 = t * t;
//			float t3 = t2 * t;
//
//			// calculate hermite basis
//			float h1 =  2.0f * t3 - 3.0f * t2 + 1.0f;
//			float h2 = -2.0f * t3 + 3.0f * t2;
//			float h3 =         t3 - 2.0f * t2 + t;
//			float h4 =         t3 -        t2;
//
//			// do hermite interpolation
//			pos[0] = h1 * p0->key[0] + h3 * m0->tangentOut[0] + h2 * p1->key[0] + h4 * m1->tangentIn[0];
//			pos[1] = h1 * p0->key[1] + h3 * m0->tangentOut[1] + h2 * p1->key[1] + h4 * m1->tangentIn[1];
//			pos[2] = h1 * p0->key[2] + h3 * m0->tangentOut[2] + h2 * p1->key[2] + h4 * m1->tangentIn[2];
//		}
//	}
//
//	vec4_t quat = { 0.0f, 0.0f, 0.0f, 1.0f };
//	int numRotationKeys = (int) joint->rotationKeys.size();
//	if (numRotationKeys > 0)
//	{
//		int i1 = -1;
//		int i2 = -1;
//
//		// find the two keys, where "frame" is in between for the rotation channel
//		for (int i = 0; i < (numRotationKeys - 1); i++)
//		{
//			if (frame >= joint->rotationKeys[i].time && frame < joint->rotationKeys[i + 1].time)
//			{
//				i1 = i;
//				i2 = i + 1;
//				break;
//			}
//		}
//
//		// if there are no such keys
//		if (i1 == -1 || i2 == -1)
//		{
//			// either take the first key
//			if (frame < joint->rotationKeys[0].time)
//			{
//				AngleQuaternion(joint->rotationKeys[0].key, quat);
//			}
//
//			// or the last key
//			else if (frame >= joint->rotationKeys[numRotationKeys - 1].time)
//			{
//				AngleQuaternion(joint->rotationKeys[numRotationKeys - 1].key, quat);
//			}
//		}
//
//		// there are such keys, so do the quaternion slerp interpolation
//		else
//		{
//			float t = (frame - joint->rotationKeys[i1].time) / (joint->rotationKeys[i2].time - joint->rotationKeys[i1].time);
//			vec4_t q1;
//			AngleQuaternion(joint->rotationKeys[i1].key, q1);
//			vec4_t q2;
//			AngleQuaternion(joint->rotationKeys[i2].key, q2);
//			QuaternionSlerp(q1, q2, t, quat);
//		}
//	}
//
//	// make a matrix from pos/quat
//	float matAnimate[3][4];
//	QuaternionMatrix(quat, matAnimate);
//	matAnimate[0][3] = pos[0];
//	matAnimate[1][3] = pos[1];
//	matAnimate[2][3] = pos[2];
//
//	// animate the local joint matrix using: matLocal = matLocalSkeleton * matAnimate
//	R_ConcatTransforms(joint->matLocalSkeleton, matAnimate, joint->matLocal);
//
//	// build up the hierarchy if joints
//	// matGlobal = matGlobal(parent) * matLocal
//	if (joint->parentIndex == -1)
//	{
//		memcpy(joint->matGlobal, joint->matLocal, sizeof(joint->matGlobal));
//	}
//	else
//	{
//		ms3d_joint_t *parentJoint = &joints[joint->parentIndex];
//		R_ConcatTransforms(parentJoint->matGlobal, joint->matLocal, joint->matGlobal);
//	}
//}
//
//void msModel::TransformVertex(const ms3d_vertex_t *vertex, vec3_t out) const
//{
//	int jointIndices[4], jointWeights[4];
//	FillJointIndicesAndWeights(vertex, jointIndices, jointWeights);
//
//	if (jointIndices[0] < 0 || jointIndices[0] >= (int) joints.size() || m_currentTime < 0.0f)
//	{
//		out[0] = vertex->vertex[0];
//		out[1] = vertex->vertex[1];
//		out[2] = vertex->vertex[2];
//	}
//	else
//	{
//		// count valid weights
//		int numWeights = 0;
//		for (int i = 0; i < 4; i++)
//		{
//			if (jointWeights[i] > 0 && jointIndices[i] >= 0 && jointIndices[i] < (int) joints.size())
//				++numWeights;
//			else
//				break;
//		}
//
//		// init
//		out[0] = 0.0f;
//		out[1] = 0.0f;
//		out[2] = 0.0f;
//
//		float weights[4] = { (float) jointWeights[0] / 100.0f, (float) jointWeights[1] / 100.0f, (float) jointWeights[2] / 100.0f, (float) jointWeights[3] / 100.0f };
//		if (numWeights == 0)
//		{
//			numWeights = 1;
//			weights[0] = 1.0f;
//		}
//		// add weighted vertices
//		for (int i = 0; i < numWeights; i++)
//		{
//			const ms3d_joint_t *joint = &joints[jointIndices[i]];
//			vec3_t tmp, vert;
//			VectorITransform(vertex->vertex, joint->matGlobalSkeleton, tmp);
//			VectorTransform(tmp, joint->matGlobal, vert);
//
//			out[0] += vert[0] * weights[i];
//			out[1] += vert[1] * weights[i];
//			out[2] += vert[2] * weights[i];
//		}
//	}
//}
//
//void msModel::TransformNormal(const ms3d_vertex_t *vertex, const vec3_t normal, vec3_t out) const
//{
//	int jointIndices[4], jointWeights[4];
//	FillJointIndicesAndWeights(vertex, jointIndices, jointWeights);
//
//	if (jointIndices[0] < 0 || jointIndices[0] >= (int) joints.size() || m_currentTime < 0.0f)
//	{
//		out[0] = normal[0];
//		out[1] = normal[1];
//		out[2] = normal[2];
//	}
//	else
//	{
//		// count valid weights
//		int numWeights = 0;
//		for (int i = 0; i < 4; i++)
//		{
//			if (jointWeights[i] > 0 && jointIndices[i] >= 0 && jointIndices[i] < (int) joints.size())
//				++numWeights;
//			else
//				break;
//		}
//
//		// init
//		out[0] = 0.0f;
//		out[1] = 0.0f;
//		out[2] = 0.0f;
//
//		float weights[4] = { (float) jointWeights[0] / 100.0f, (float) jointWeights[1] / 100.0f, (float) jointWeights[2] / 100.0f, (float) jointWeights[3] / 100.0f };
//		if (numWeights == 0)
//		{
//			numWeights = 1;
//			weights[0] = 1.0f;
//		}
//		// add weighted vertices
//		for (int i = 0; i < numWeights; i++)
//		{
//			const ms3d_joint_t *joint = &joints[jointIndices[i]];
//			vec3_t tmp, norm;
//			VectorIRotate(normal, joint->matGlobalSkeleton, tmp);
//			VectorRotate(tmp, joint->matGlobal, norm);
//
//			out[0] += norm[0] * weights[i];
//			out[1] += norm[1] * weights[i];
//			out[2] += norm[2] * weights[i];
//		}
//	}
//}
//
//void msModel::FillJointIndicesAndWeights(const ms3d_vertex_t *vertex, int jointIndices[4], int jointWeights[4]) const
//{
//	jointIndices[0] = vertex->boneId;
//	jointIndices[1] = vertex->boneIds[0];
//	jointIndices[2] = vertex->boneIds[1];
//	jointIndices[3] = vertex->boneIds[2];
//	jointWeights[0] = 100;
//	jointWeights[1] = 0;
//	jointWeights[2] = 0;
//	jointWeights[3] = 0;
//	if (vertex->weights[0] != 0 || vertex->weights[1] != 0 || vertex->weights[2] != 0)
//	{
//		jointWeights[0] = vertex->weights[0];
//		jointWeights[1] = vertex->weights[1];
//		jointWeights[2] = vertex->weights[2];
//		jointWeights[3] = 100 - (vertex->weights[0] + vertex->weights[1] + vertex->weights[2]);
//	}
//}

//-----------------------------------//

} // end namespace