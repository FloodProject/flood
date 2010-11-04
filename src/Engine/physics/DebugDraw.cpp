/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_PHYSICS_BULLET

#include "vapor/physics/DebugDraw.h"
#include "vapor/physics/Convert.h"
#include "vapor/render/Renderable.h"

namespace vapor {

//-----------------------------------//

BulletDebugDrawer::BulletDebugDrawer()
{
	vb = new VertexBuffer(BufferUsage::Dynamic, BufferAccess::Write);
	clearBuffer();

	material = new Material("DebugDrawer");
	material->setProgram("Diffuse");
	material->setDepthCompare(DepthCompare::LessOrEqual);

	renderable = new Renderable(PolygonType::Lines);
	renderable->setVertexBuffer(vb);
	renderable->setMaterial(material);
}

//-----------------------------------//

void BulletDebugDrawer::clearBuffer()
{
	std::vector<Vector3> pos;
	vb->set(VertexAttribute::Position, pos);
	
	std::vector<Vector3> colors;
	vb->set( VertexAttribute::Color, colors);
}

//-----------------------------------//

void BulletDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to,
	const btVector3 &color)
{
	std::vector<Vector3>& pos = vb->getAttribute(VertexAttribute::Position);
	pos.push_back(Convert::fromBullet(from));
	pos.push_back(Convert::fromBullet(to));

	std::vector<Vector3>& colors = vb->getAttribute(VertexAttribute::Color);
	colors.push_back(Convert::fromBullet(color));
	colors.push_back(Convert::fromBullet(color));
}

//-----------------------------------//

void BulletDebugDrawer::drawContactPoint(const btVector3 &PointOnB,
	const btVector3 &normalOnB, btScalar distance,
	int lifeTime, const btVector3 &color)
{

}

//-----------------------------------//

void BulletDebugDrawer::reportErrorWarning (const char *warningString)
{
	Log::error( warningString );
}

//-----------------------------------//

void BulletDebugDrawer::draw3dText (const btVector3 &location, const char *textString)
{

}

//-----------------------------------//

void BulletDebugDrawer::setDebugMode(int debugMode)
{

}

//-----------------------------------//

int	BulletDebugDrawer::getDebugMode() const
{
	return 1;
}

//-----------------------------------//

} // end namespace

#endif