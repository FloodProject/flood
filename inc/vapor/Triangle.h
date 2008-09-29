#pragma once

#include "Renderable.h"
#include "Vector3.h"

using namespace math

namespace fuck {
namespace renderer {

class Triangle : Renderable
{
	Vertex vertices[3];

	
  public:
/** constructor - update the Mpoints p1, p2 and p3*/
  Triangle(Vertex* const p1,Vertex* const p2,Vertex* const p3);
  /** destructor - you must remove neighbourhood relation between*/
	  ~Triangle(void);
  /** Declared but undefined : prevents from using it */
	  Triangle(Triangle & t);
 /** Declared but undefined : prevents from using it */
	  Triangle operator=(Triangle &t); 

  /** used to store extra data attached to the triangle */
	  vector<double> data; 
  /** accessor for vertices */
	  Vertex * const get_vertice(const int i) const;
  /** true if the triangle has been well oriented (will change this) */
	  bool oriented;

  /** centroid of the triangle */
	  const Pt centroid() const;
  /** normal of the oriented triangle */
	  const Vector3 normal() const;
  /** change the orientation of the triangle */
	  void swap();
  /** check if two triangles are adjacent.
    0 if they are not
    1 if they are and if they are well oriented respectively to each other
    2 if they are and if they are bad oriented respectively to each other */
	  const int operator <(const Triangle *const t) const;

/** comparaison operator - compares the vertices */
  const bool operator ==(const Triangle & t) const
  {
		if( vertices[1] == t.get_vertice(1)
			&& vertices[2] == t.get_vertice(2)
			&& vertices[3] == t.get_vertice(3))
			return true;
		else
			return false;
  }
  /** check if two triangles are intersecting each other */
  const bool intersect(const Triangle & t) const
  {
		if(
  }
};

}
}
