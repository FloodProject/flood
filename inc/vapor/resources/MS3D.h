/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Mesh.h"
#include "vapor/vfs/File.h"

namespace vapor {

//-----------------------------------//

struct ms3d_vertex_t;
struct ms3d_triangle_t;
struct ms3d_group_t;
struct ms3d_material_t;

//-----------------------------------//

/**
 * Represents Milkshape 3D meshes. These can hold vertex, animation, and 
 * material data, are relatively easy to parse and use, and are quite
 * well documented and lightweight.
 */

class MS3D : public Mesh
{
public:

	MS3D();

	// Loads the geometry data into the mesh.
	bool load(const File& file);

protected:

	bool read(const File& file);
	void clear();

	// Builds the vertex and index buffers representing the mesh.
	void build( std::vector<RenderablePtr>& renderables );
	bool hasMaterial( const ms3d_group_t& g );

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
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( MS3D );

//-----------------------------------//

} // end namespace