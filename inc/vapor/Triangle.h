#pragma once

#include "vapor/Renderable.h"
#include "vapor/Vector3.h"

using namespace math

namespace vapor {
	namespace render {

class Triangle : Renderable
{
public:

	// constructor - update the Mpoints p1, p2 and p3
	Triangle(const Vertex* p1, const Vertex* p2, const Vertex* p3);
	
	// destructor - you must remove neighbourhood relation between
	~Triangle();
  
	// Declared but undefined : prevents from using it
	Triangle(Triangle & t);

	// Declared but undefined : prevents from using it
	Triangle operator=(Triangle &t); 

	// used to store extra data attached to the triangle
	vector<double> data; 
  
	// accessor for vertices
	Vertex * const get_vertice(const int i) const;

	// centroid of the triangle
	const Pt centroid() const;
  
	// normal of the oriented triangle
	const Vector3 normal() const;
  
	// change the orientation of the triangle 
	void swap();
  
	/** check if two triangles are adjacent.
    0 if they are not
    1 if they are and if they are well oriented respectively to each other
    2 if they are and if they are bad oriented respectively to each other */
	const int operator <(const Triangle *const t) const;

	// comparison operator - compares the vertices
	const bool operator ==(const Triangle & t) const
	{
		if( vertices[1] == t.get_vertice(1)
			&& vertices[2] == t.get_vertice(2)
			&& vertices[3] == t.get_vertice(3))
			return true;
		else
			return false;
  }
  
	// check if two triangles are intersecting each other
	const bool intersect(const Triangle & t) const
	{
		
	}

private:

	Vertex vertices[3];

	// true if the triangle has been well oriented (will change this)
	bool oriented;
};

} } // end namespaces 
