/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#include "Math/Vector.h"
#include "Math/Ray.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class NodeAABB;

class NodeInterface
{
public:
	virtual NodeAABB* getNode() = 0;
	virtual void getFaceNormal(uint32 tri,float *faceNormal) = 0;
};

class BoundingBoxTree
{
public:

	virtual bool raycast(const Ray& ,float *hitLocation,float *hitNormal,float *hitDistance) = 0;
	
	virtual const float * getBoundMin() const = 0;
	virtual const float * getBoundMax() const = 0;
};

//BoundingBoxTree* CreateBoundingBoxTreeFromMesh(const MeshPtr&);
BoundingBoxTree* CreateBoundingBoxTree(const std::vector<Vector3>& vertices, std::vector<uint16> indices);

#if 0
RaycastMesh * CreateRaycastMesh(RmUint32 vcount,                // The number of vertices in the source triangle mesh
                                                                const float *vertices,         // The array of vertex positions in the format x1,y1,z1..x2,y2,z2.. etc.
                                                                RmUint32 tcount,                // The number of triangles in the source triangle mesh
                                                                const RmUint32 *indices, // The triangle indices in the format of i1,i2,i3 ... i4,i5,i6, ...
                                                                RmUint32 maxDepth=15,   // Maximum recursion depth for the triangle mesh.
                                                                RmUint32 minLeafSize=4, // minimum triangles to treat as a 'leaf' node.
                                                                float  minAxisSize=0.01f       // once a particular axis is less than this size, stop sub-dividing.
                                                                );
#endif

//-----------------------------------//

NAMESPACE_ENGINE_END