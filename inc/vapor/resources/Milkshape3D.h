/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <cstdio>
#include <vector>
#include <string>

#include "vapor/Engine.h"

#include "vapor/resources/Resource.h"

using namespace std;

namespace vapor {
	namespace resources {

//const int MAX_VERTICES = 65534;
//const int MAX_TRIANGLES = 65534;
//const int MAX_GROUPS = 255;
//const int MAX_MATERIALS = 128;
//const int MAX_JOINTS = 128;
//const int MAX_TEXTURE_FILENAME_SIZE = 128;
//
//#define SELECTED		1
//#define HIDDEN			2
//#define SELECTED2		4
//#define DIRTY			8
//#define ISKEY			16
//#define NEWLYCREATED	32
//#define MARKED			64
//
//#define SPHEREMAP		0x80
//#define HASALPHA		0x40
//#define COMBINEALPHA    0x20

//enum TransparencyMode
//{
//	TRANSPARENCY_MODE_SIMPLE,			
//	TRANSPARENCY_MODE_DEPTHSORTEDTRIANGLES,
//	TRANSPARENCY_MODE_ALPHAREF
//};
//
#define MAX_VERTICES				65534
#define MAX_TRIANGLES				65534
#define MAX_GROUPS					255
#define MAX_MATERIALS				128
#define MAX_JOINTS					128
#define MAX_TEXTURE_FILENAME_SIZE	128

#include <pshpack1.h>

struct ms3d_header_t
{
	char    id[10];
	long    version;
};

struct ms3d_vertex_t
{
	byte	flags;
	float	vertex[3];
	char	boneId;
	byte referenceCount;
	char boneIds[3];
	//unsigned char weights[3];
	//unsigned int extra;
	//float renderColor[3];
};

struct ms3d_triangle_t
{
	ushort	flags;
	ushort	vertexIndices[3];
	float	vertexNormals[3][3];
	float	s[3];
	float	t[3];
	float	normal[3];
	byte	smoothingGroup;
	byte	groupIndex;
};

struct ms3d_group_t
{
	unsigned char flags;
	char name[32];
	std::vector<unsigned short> triangleIndices;
	char materialIndex;
	std::vector<char> comment;
};

struct ms3d_material_t
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
};

#include <poppack.h>


class Milkshape3D : public Resource
{

public:

	Milkshape3D(const string &filename);
	virtual ~Milkshape3D();

	//int get_num_groups() const;
	//ms3d_group_t *get_group(int index);

	//int get_num_triangles() const;
	//ms3d_triangle_t *get_triangle(int index);

	//int get_num_vertices() const;
	//ms3d_vertex_t *get_vertex(int index);

	//int get_num_materials() const;
	//ms3d_material_t *get_material(int index);

private:

	bool load(const string &filename);
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
	vector<ms3d_vertex_t> m_vertices;
	vector<ms3d_triangle_t> m_triangles;
	//vector<ms3d_group_t> m_groups;
	//vector<ms3d_material_t> m_materials;
	//vector<char> m_comment;
};

} } // end namespaces
