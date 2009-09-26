/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/resources/Resource.h"

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
	unsigned char flags;
	char name[32];
	std::vector<unsigned short> triangleIndices;
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

#pragma pack( pop )

//-----------------------------------//

class MS3D : public Resource
{

public:

	MS3D(const std::string &filename);
	virtual ~MS3D();

	ResourceGroup::Enum getResourceGroup() const { return ResourceGroup::Meshes; }

	//int get_num_groups() const;
	//ms3d_group_t *get_group(int index);

	//int get_num_triangles() const;
	//ms3d_triangle_t *get_triangle(int index);

	//int get_num_vertices() const;
	//ms3d_vertex_t *get_vertex(int index);

	//int get_num_materials() const;
	//ms3d_material_t *get_material(int index);

private:

	bool load(const std::string &filename);
	void clear();

	bool read_header();
	void read_vertices();
	void read_triangles();
	//void read_groups();
	//void read_materials();
	//void read_animatin();
	//void read_joints();
	//void read_comments();

private:

	FILE *fp;

	std::vector<ms3d_vertex_t> m_vertices;
	std::vector<ms3d_triangle_t> m_triangles;
	std::vector<ms3d_group_t> m_groups;
	std::vector<ms3d_material_t> m_materials;
	std::vector<char> m_comment;
};

//-----------------------------------//

} } // end namespaces