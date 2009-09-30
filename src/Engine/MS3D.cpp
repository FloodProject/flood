/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

// based on official Milkshape3D viewer source

#include "vapor/PCH.h"

#include "vapor/resources/MS3D.h"
#include "vapor/vfs/File.h"

using namespace vapor::render;
using namespace vapor::math;
using namespace vapor::scene;

namespace vapor {
	namespace resources {

//-----------------------------------//

const int MAX_VERTICES				= 65534;
const int MAX_TRIANGLES				= 65534;
const int MAX_GROUPS				= 255;
const int MAX_MATERIALS				= 128;
const int MAX_JOINTS				= 128;
const int MAX_TEXTURE_FILENAME_SIZE = 128;

const int SELECTED		= 1;
const int HIDDEN		= 2;
const int SELECTED2		= 4;
const int DIRTY			= 8;
const int ISKEY			= 16;
const int NEWLYCREATED	= 32;
const int MARKED		= 64;

const int SPHEREMAP		= 0x80;
const int HASALPHA		= 0x40;
const int COMBINEALPHA	= 0x20;

enum TransparencyMode
{
	TRANSPARENCY_MODE_SIMPLE,			
	TRANSPARENCY_MODE_DEPTHSORTEDTRIANGLES,
	TRANSPARENCY_MODE_ALPHAREF
};

//-----------------------------------//

#pragma pack( push, 1 )

struct ms3d_header_t
{
	char    id[10];
	long    version;
};

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_vertex_t
{
	byte	flags;
	float	vertex[3];
	char	boneId;
	byte referenceCount;
	char boneIds[3];
	//unsigned char weights[3];
	//unsigned int extra;
	//float renderColor[3];
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_triangle_t
{
	ushort	flags;
	ushort	vertexIndices[3];
	float	vertexNormals[3][3];
	float	s[3];
	float	t[3];
	float	normal[3];
	byte	smoothingGroup;
	byte	groupIndex;
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_group_t
{
	byte flags;
	char name[32];
	std::vector<ushort> triangleIndices;
	char materialIndex;
	std::vector<char> comment;
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_material_t
{
	char			name[32];
	float			ambient[4];
	float			diffuse[4];
	float			specular[4];
	float			emissive[4];
	float			shininess;
    float			transparency;
	byte			mode;
	char			texture[MAX_TEXTURE_FILENAME_SIZE];
    char			alphamap[MAX_TEXTURE_FILENAME_SIZE];
	byte			id;
	std::vector<char>	comment;
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_keyframe_t
{
	float time;
	float key[3];
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_tangent_t
{
	float tangentIn[3];
	float tangentOut[3];
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_joint_t
{
	byte flags;
	char name[32];
	char parentName[32];

	float rot[3];
	float pos[3];

	std::vector<ms3d_keyframe_t> rotationKeys;
	std::vector<ms3d_keyframe_t> positionKeys;
	std::vector<ms3d_tangent_t> tangents;

	std::vector<char> comment;
	float color[3];

	// used for rendering
	int parentIndex;
	float matLocalSkeleton[3][4];
	float matGlobalSkeleton[3][4];

	float matLocal[3][4];
	float matGlobal[3][4];
} VAPOR_ALIGN_END(1);

#pragma pack( pop )

//-----------------------------------//

MS3D::MS3D(const std::string& filename)
{
	load(filename);
	build();
}

//-----------------------------------//

MS3D::~MS3D()
{
	
}

//-----------------------------------//

bool MS3D::load(const std::string& filename)
{
#ifdef VAPOR_COMPILER_MSVC
	// disable Visual C++ fopen deprecation warning
	#pragma warning(disable : 4996)
#endif

	fp = fopen(filename.c_str(), "rb");
	if (!fp) return false;
	
	if(!read_header())
		goto cleanup;
	
	read_vertices();
	read_triangles();
	read_groups();
	read_materials();
	//read_animation();
	//read_joints();
	//read_comments();

	fclose(fp);
	return true;

cleanup:

	fclose(fp);
	return false;
}

//-----------------------------------//

void MS3D::build()
{
	bool has_material = false;

	foreach( const ms3d_group_t& g, m_groups )
	{
		// In case this group doesn't have geometry, then no need to process it.
		if( g.triangleIndices.empty() ) continue;
		
		// Let's check if we have a valid material in the structure.
		if( (g.materialIndex != -1) 
			&& (g.materialIndex >= 0) 
			&& (g.materialIndex < signed(m_materials.size())) )
		{
			has_material = true;
		}
		else
		{
			has_material = false;
		}
		
		RenderablePtr rend( new Renderable() );

		// MS3D meshes represent everything as triangles.
		rend->setType( Primitive::Triangles );

		// Vertex data
		std::vector< Vector3 > vb_v;

		// Tex coord data
		std::vector< Vector3 > vb_tc;

		// Let's process all the triangles of this group.
		foreach( const ushort t_ind, g.triangleIndices )
		{
			const ms3d_triangle_t& t = m_triangles[t_ind];
			
			foreach( const ushort& v_ind, t.vertexIndices ) 
			{
				Vector3 vec;
				
				vec.x = m_vertices[v_ind].vertex[0];
				vec.y = m_vertices[v_ind].vertex[1];
				vec.z = m_vertices[v_ind].vertex[2];
				
				vb_v.push_back( vec );
			}

			for( int i = 0; i < 3; i++ )
			{
				Vector3 st;

				st.x = t.s[i];
				st.y = t.t[i];
				st.z = 0;
				
				vb_tc.push_back( st );	
			}
		}

		// Material
		if( has_material )
		{
			const ms3d_material_t mt = m_materials[g.materialIndex];

			MaterialPtr mat( new Material( mt.name ) );

			if( strlen(mt.texture) > 0 )
			{
				mat->setTexture( 0, mt.texture );
			}

			rend->setMaterial( mat );
		}

		// Vertex buffers
		
		VertexBufferPtr vb( new VertexBuffer() );
		vb->set( VertexAttribute::Vertex, vb_v );
		vb->set( VertexAttribute::MultiTexCoord0, vb_tc );
		
		rend->setVertexBuffer( vb );

		//// Index buffers
		//std::vector< ushort > vb_i;
		//foreach( const ms3d_triangle_t& t, m_triangles )
		//{
		//	vb_i.push_back( t.vertexIndices[0] );
		//	vb_i.push_back( t.vertexIndices[1] );
		//	vb_i.push_back( t.vertexIndices[2] );
		//}

		//IndexBufferPtr ib( new IndexBuffer() );
		//ib->set( vb_i );
		//rend->setIndexBuffer( ib );

		addRenderable( rend );
	}
}

//-----------------------------------//

bool MS3D::read_header()
{
	ms3d_header_t header;

	fread(&header, 1, sizeof(ms3d_header_t), fp);

	if (strncmp(header.id, "MS3D000000", 10) != 0) 
		return false;

	if (header.version != 4) 
		return false;

	return true;
}

//-----------------------------------//

void MS3D::read_vertices()
{
	ushort num_vertices;

	fread(&num_vertices, sizeof(ushort), 1, fp);
	m_vertices.resize(num_vertices);

	for (int i = 0; i < num_vertices; i++)
	{
		fread(&m_vertices[i].flags, sizeof(byte), 1, fp);
		fread(&m_vertices[i].vertex, sizeof(float), 3, fp);
		fread(&m_vertices[i].boneId, sizeof(char), 1, fp);
		fread(&m_vertices[i].referenceCount, sizeof(byte), 1, fp);
	}
}

//-----------------------------------//

void MS3D::read_triangles()
{
	ushort num_triangles;
	
	fread(&num_triangles, sizeof(ushort), 1, fp);
	m_triangles.resize(num_triangles);
	
	for (int i = 0; i < num_triangles; i++)
	{
		fread(&m_triangles[i].flags, sizeof(ushort), 1, fp);
		fread(m_triangles[i].vertexIndices, sizeof(ushort), 3, fp);
		fread(m_triangles[i].vertexNormals, sizeof(float), 3 * 3, fp);
	    fread(m_triangles[i].s, sizeof(float), 3, fp);
	    fread(m_triangles[i].t, sizeof(float), 3, fp);
		fread(&m_triangles[i].smoothingGroup, sizeof(byte), 1, fp);
		fread(&m_triangles[i].groupIndex, sizeof(byte), 1, fp);

		// TODO: calculate triangle normal
	}
}

//-----------------------------------//

void MS3D::read_groups()
{
	// groups
	ushort numGroups;
	
	fread(&numGroups, sizeof(ushort), 1, fp);
	m_groups.resize(numGroups);
	
	for (int i = 0; i < numGroups; i++)
	{
		fread(&m_groups[i].flags, sizeof(byte), 1, fp);
		fread(m_groups[i].name, sizeof(char), 32, fp);

		ushort numGroupTriangles;
		fread(&numGroupTriangles, sizeof(ushort), 1, fp);
		m_groups[i].triangleIndices.resize(numGroupTriangles);
		
		if (numGroupTriangles > 0)
		{
			fread(&m_groups[i].triangleIndices[0], sizeof(ushort), numGroupTriangles, fp);
		}

		fread(&m_groups[i].materialIndex, sizeof(byte), 1, fp);
	}
}

//-----------------------------------//

void MS3D::read_materials()
{
	// materials
	ushort numMaterials;
	fread(&numMaterials, sizeof(ushort), 1, fp);
	m_materials.resize(numMaterials);
	
	for (int i = 0; i < numMaterials; i++)
	{
		fread(m_materials[i].name, sizeof(char), 32, fp);
		fread(&m_materials[i].ambient, sizeof(float), 4, fp);
		fread(&m_materials[i].diffuse, sizeof(float), 4, fp);
		fread(&m_materials[i].specular, sizeof(float), 4, fp);
		fread(&m_materials[i].emissive, sizeof(float), 4, fp);
		fread(&m_materials[i].shininess, sizeof(float), 1, fp);
		fread(&m_materials[i].transparency, sizeof(float), 1, fp);
		fread(&m_materials[i].mode, sizeof(byte), 1, fp);
		fread(m_materials[i].texture, sizeof(char), MAX_TEXTURE_FILENAME_SIZE, fp);
		fread(m_materials[i].alphamap, sizeof(char), MAX_TEXTURE_FILENAME_SIZE, fp);

		// set alpha
		m_materials[i].ambient[3] = m_materials[i].transparency;
		m_materials[i].diffuse[3] = m_materials[i].transparency;
		m_materials[i].specular[3] = m_materials[i].transparency;
		m_materials[i].emissive[3] = m_materials[i].transparency;
	}
}

//-----------------------------------//

//void Milkshape3D::read_animation()
//{
//	// animation
//	fread(&m_animationFps, sizeof(float), 1, fp);
//	if (m_animationFps < 1.0f)
//		m_animationFps = 1.0f;
//	fread(&m_currentTime, sizeof(float), 1, fp);
//	fread(&m_totalFrames, sizeof(int), 1, fp);
//}
//
//
//void Milkshape3D::read_joints()
//{
//	// joints
//	ushort numJoints;
//	fread(&numJoints, sizeof(ushort), 1, fp);
//	m_joints.resize(numJoints);
//	for (i = 0; i < numJoints; i++)
//	{
//		fread(&m_joints[i].flags, sizeof(byte), 1, fp);
//		fread(m_joints[i].name, sizeof(char), 32, fp);
//		fread(m_joints[i].parentName, sizeof(char), 32, fp);
//        fread(m_joints[i].rot, sizeof(float), 3, fp);
//        fread(m_joints[i].pos, sizeof(float), 3, fp);
//    
//		ushort numKeyFramesRot;
//		fread(&numKeyFramesRot, sizeof(ushort), 1, fp);
//		m_joints[i].rotationKeys.resize(numKeyFramesRot);
//
//		ushort numKeyFramesPos;
//		fread(&numKeyFramesPos, sizeof(ushort), 1, fp);
//		m_joints[i].positionKeys.resize(numKeyFramesPos);
//
//		// the frame time is in seconds, so multiply it by the animation fps, to get the frames
//		// rotation channel
//		for (j = 0; j < numKeyFramesRot; j++)
//		{
//			fread(&m_joints[i].rotationKeys[j].time, sizeof(float), 1, fp);
//			fread(&m_joints[i].rotationKeys[j].key, sizeof(float), 3, fp);
//			m_joints[i].rotationKeys[j].time *= m_animationFps;
//		}
//
//		// translation channel
//		for (j = 0; j < numKeyFramesPos; j++)
//		{
//			fread(&m_joints[i].positionKeys[j].time, sizeof(float), 1, fp);
//			fread(&m_joints[i].positionKeys[j].key, sizeof(float), 3, fp);
//			m_joints[i].positionKeys[j].time *= m_animationFps;
//		}
//	}
//}
//
//
//void Milkshape3D::read_comments()
//{
//	// comments
//	long filePos = ftell(fp);
//	if (filePos < fileSize)
//	{
//		int subVersion = 0;
//		fread(&subVersion, sizeof(int), 1, fp);
//		if (subVersion == 1)
//		{
//			int numComments = 0;
//			size_t commentSize = 0;
//
//			// group comments
//			fread(&numComments, sizeof(int), 1, fp); 
//			for (i = 0; i < numComments; i++)
//			{
//				int index;
//				fread(&index, sizeof(int), 1, fp);
//				std::vector<char> comment;
//				fread(&commentSize, sizeof(size_t), 1, fp);
//				comment.resize(commentSize);
//				if (commentSize > 0)
//					fread(&comment[0], sizeof(char), commentSize, fp);
//				if (index >= 0 && index < (int) m_groups.size())
//					m_groups[index].comment = comment;
//			}
//
//			// material comments
//			fread(&numComments, sizeof(int), 1, fp); 
//			for (i = 0; i < numComments; i++)
//			{
//				int index;
//				fread(&index, sizeof(int), 1, fp);
//				std::vector<char> comment;
//				fread(&commentSize, sizeof(size_t), 1, fp);
//				comment.resize(commentSize);
//				if (commentSize > 0)
//					fread(&comment[0], sizeof(char), commentSize, fp);
//				if (index >= 0 && index < (int) m_materials.size())
//					m_materials[index].comment = comment;
//			}
//
//			// joint comments
//			fread(&numComments, sizeof(int), 1, fp); 
//			for (i = 0; i < numComments; i++)
//			{
//				int index;
//				fread(&index, sizeof(int), 1, fp);
//				std::vector<char> comment;
//				fread(&commentSize, sizeof(size_t), 1, fp);
//				comment.resize(commentSize);
//				if (commentSize > 0)
//					fread(&comment[0], sizeof(char), commentSize, fp);
//				if (index >= 0 && index < (int) m_joints.size())
//					m_joints[index].comment = comment;
//			}
//
//			// model comments
//			fread(&numComments, sizeof(int), 1, fp);
//			if (numComments == 1)
//			{
//				std::vector<char> comment;
//				fread(&commentSize, sizeof(size_t), 1, fp);
//				comment.resize(commentSize);
//				if (commentSize > 0)
//					fread(&comment[0], sizeof(char), commentSize, fp);
//				m_comment = comment;
//			}
//		}
//		else
//		{
//			// "Unknown subversion for comments %d\n", subVersion);
//		}
//	}
//}
//
//
//void Milkshape3D::read_extra()
//{
//	// vertex extra
//	filePos = ftell(fp);
//	if (filePos < fileSize)
//	{
//		int subVersion = 0;
//		fread(&subVersion, sizeof(int), 1, fp);
//		if (subVersion == 2)
//		{
//			for (int i = 0; i < numVertices; i++)
//			{
//				fread(&m_vertices[i].boneIds[0], sizeof(char), 3, fp);
//				fread(&m_vertices[i].weights[0], sizeof(byte), 3, fp);
//				fread(&m_vertices[i].extra, sizeof(unsigned int), 1, fp);
//			}
//		}
//		else if (subVersion == 1)
//		{
//			for (int i = 0; i < numVertices; i++)
//			{
//				fread(&m_vertices[i].boneIds[0], sizeof(char), 3, fp);
//				fread(&m_vertices[i].weights[0], sizeof(byte), 3, fp);
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
//		fread(&subVersion, sizeof(int), 1, fp);
//		if (subVersion == 1)
//		{
//			for (int i = 0; i < numJoints; i++)
//			{
//				fread(&m_joints[i].color, sizeof(float), 3, fp);
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
//		fread(&subVersion, sizeof(int), 1, fp);
//		if (subVersion == 1)
//		{
//			fread(&m_jointSize, sizeof(float), 1, fp);
//			fread(&m_transparencyMode, sizeof(int), 1, fp);
//			fread(&m_alphaRef, sizeof(float), 1, fp);
//		}
//		else
//		{
//			//"Unknown subversion for model extra %d\n", subVersion);
//		}
//	}
//}

//
//void msModel::Clear()
//{
//	m_vertices.clear();
//	m_triangles.clear();
//	m_groups.clear();
//	m_materials.clear();
//	m_animationFps = 24.0f;
//	m_currentTime = 1.0f;
//	m_totalFrames = 30;
//	m_joints.clear();
//	m_comment.clear();
//	m_jointSize = 1.0f;
//	m_transparencyMode = TRANSPARENCY_MODE_SIMPLE;
//	m_alphaRef = 0.5f;
//}
//
//int msModel::GetNumGroups() const
//{
//	return (int) m_groups.size();
//}
//
//ms3d_group_t *msModel::GetGroup(int index)
//{
//	return &m_groups[index];
//}
//
//int msModel::GetNumTriangles() const
//{
//	return (int) m_triangles.size();
//}
//
//ms3d_triangle_t *msModel::GetTriangle(int index)
//{
//	return &m_triangles[index];
//}
//
//int msModel::GetNumVertices() const
//{
//	return (int) m_vertices.size();
//}
//
//ms3d_vertex_t *msModel::GetVertex(int index)
//{
//	return &m_vertices[index];
//}
//
//int msModel::GetNumMaterials() const
//{
//	return (int) m_materials.size();
//}
//
//ms3d_material_t *msModel::GetMaterial(int index)
//{
//	return &m_materials[index];
//}
//
//int msModel::GetNumJoints() const
//{
//	return (int) m_joints.size();
//}
//
//ms3d_joint_t *msModel::GetJoint(int index)
//{
//	return &m_joints[index];
//}
//
//float msModel::GetJointSize() const
//{
//	return m_jointSize;
//}
//
//int msModel::GetTransparencyMode() const
//{
//	return m_transparencyMode;
//}
//
//float msModel::GetAlphaRef() const
//{
//	return m_alphaRef;
//}
//
//int msModel::FindJointByName(const char *name)
//{
//	for (size_t i = 0; i < m_joints.size(); i++)
//	{
//		if (strcmp(m_joints[i].name, name) == 0)
//			return i;
//	}
//
//	return -1;
//}
//
//void msModel::SetupJoints()
//{
//	for (size_t i = 0; i < m_joints.size(); i++)
//	{
//		ms3d_joint_t *joint = &m_joints[i];
//		joint->parentIndex = FindJointByName(joint->parentName);
//	}
//
//	for (size_t i = 0; i < m_joints.size(); i++)
//	{
//		ms3d_joint_t *joint = &m_joints[i];
//		AngleMatrix(joint->rot, joint->matLocalSkeleton);
//		joint->matLocalSkeleton[0][3]= joint->pos[0];
//		joint->matLocalSkeleton[1][3]= joint->pos[1];
//		joint->matLocalSkeleton[2][3]= joint->pos[2];
//		
//		if (joint->parentIndex == -1)
//		{
//			memcpy(joint->matGlobalSkeleton, joint->matLocalSkeleton, sizeof(joint->matGlobalSkeleton));
//		}
//		else
//		{
//			ms3d_joint_t *parentJoint = &m_joints[joint->parentIndex];
//			R_ConcatTransforms(parentJoint->matGlobalSkeleton, joint->matLocalSkeleton, joint->matGlobalSkeleton);
//		}
//
//		SetupTangents();
//	}
//}
//
//void msModel::SetupTangents()
//{
//	for (size_t j = 0; j < m_joints.size(); j++)
//	{
//		ms3d_joint_t *joint = &m_joints[j];
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
//		for (size_t i = 0; i < m_joints.size(); i++)
//		{
//			ms3d_joint_t *joint = &m_joints[i];
//			memcpy(joint->matLocal, joint->matLocalSkeleton, sizeof(joint->matLocal));
//			memcpy(joint->matGlobal, joint->matGlobalSkeleton, sizeof(joint->matGlobal));
//		}
//	}
//	else
//	{
//		for (size_t i = 0; i < m_joints.size(); i++)
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
//	ms3d_joint_t *joint = &m_joints[index];
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
//		ms3d_joint_t *parentJoint = &m_joints[joint->parentIndex];
//		R_ConcatTransforms(parentJoint->matGlobal, joint->matLocal, joint->matGlobal);
//	}
//}
//
//float msModel::GetAnimationFps() const
//{
//	return m_animationFps;
//}
//
//float msModel::GetCurrentFrame() const
//{
//	return m_currentTime;
//}
//
//int msModel::GetTotalFrames() const
//{
//	return m_totalFrames;
//}
//
//void msModel::TransformVertex(const ms3d_vertex_t *vertex, vec3_t out) const
//{
//	int jointIndices[4], jointWeights[4];
//	FillJointIndicesAndWeights(vertex, jointIndices, jointWeights);
//
//	if (jointIndices[0] < 0 || jointIndices[0] >= (int) m_joints.size() || m_currentTime < 0.0f)
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
//			if (jointWeights[i] > 0 && jointIndices[i] >= 0 && jointIndices[i] < (int) m_joints.size())
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
//			const ms3d_joint_t *joint = &m_joints[jointIndices[i]];
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
//	if (jointIndices[0] < 0 || jointIndices[0] >= (int) m_joints.size() || m_currentTime < 0.0f)
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
//			if (jointWeights[i] > 0 && jointIndices[i] >= 0 && jointIndices[i] < (int) m_joints.size())
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
//			const ms3d_joint_t *joint = &m_joints[jointIndices[i]];
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






} } // end namespaces
