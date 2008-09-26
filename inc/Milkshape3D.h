#pragma once

#include <cstdio>
#include <vector>
#include <string>

#include "extra_shit.h"

using namespace std;

namespace vapor {
	namespace resources {

class Milkshape3D
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