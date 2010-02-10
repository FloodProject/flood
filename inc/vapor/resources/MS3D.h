/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Mesh.h"

namespace vapor { namespace resources {

//-----------------------------------//

struct ms3d_vertex_t;
struct ms3d_triangle_t;
struct ms3d_group_t;
struct ms3d_material_t;

//-----------------------------------//

/**
 * Represents MilkShape3D meshes. These can hold vertex, animation, and 
 * material data, are relatively easy to parse and use, and are quite
 * well documented and lightweight.
 */

class MS3D : public Mesh
{
public:

	MS3D(const vfs::File& file);
	virtual ~MS3D();

	// Gets the geometry for this mesh resource.
	virtual scene::GeometryPtr getGeometry();

protected:

	bool load();
	void clear();

	// Builds the vertex and index buffers representing the mesh.
	void build();

	// Reads and parses the MilkShape3D meshes structures.
	bool read_header();
	void read_vertices();
	void read_triangles();
	void read_groups();
	void read_materials();
	//void read_animatin();
	//void read_joints();
	//void read_comments();

	std::vector<byte> filebuf;
	long index;

	std::vector<ms3d_vertex_t*> m_vertices;
	std::vector<ms3d_triangle_t*> m_triangles;
	std::vector<ms3d_group_t> m_groups;
	std::vector<ms3d_material_t*> m_materials;
	char* m_comment;

	scene::GeometryPtr geometry;
};

//-----------------------------------//

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( MS3D );

//-----------------------------------//

} } // end namespaces