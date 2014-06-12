/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_PHYSICS_BULLET

#include "Engine/Physics/DebugDraw.h"
#include "Engine/Physics/Convert.h"
#include "Graphics/RenderBatch.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

BulletDebugDrawer::BulletDebugDrawer()
{
    gb = AllocateThis(GeometryBuffer, BufferUsage::Dynamic, BufferAccess::Write);
    clearBuffer();

    material = MaterialCreate(AllocatorGetHeap(), "PhysicsDebug");
    material.Resolve()->setDepthCompare(DepthCompare::LessOrEqual);

    renderable = AllocateThis(Renderable);
    renderable->setPrimitiveType(PrimitiveType::Lines);
    renderable->setGeometryBuffer(gb);
    renderable->setMaterial(material);
}

//-----------------------------------//

void BulletDebugDrawer::clearBuffer()
{
    Vector<Vector3> pos;
    gb->set(VertexAttribute::Position, pos);
    
    Vector<Vector3> colors;
    gb->set( VertexAttribute::Color, colors);
}

//-----------------------------------//

void BulletDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
#ifdef GEOMETRY_BUFFER
    Vector<Vector3>& pos = vb->getAttribute(VertexAttribute::Position);
    pos.Push(Convert::fromBullet(from));
    pos.Push(Convert::fromBullet(to));

    Vector<Vector3>& colors = vb->getAttribute(VertexAttribute::Color);
    colors.Push(Convert::fromBullet(color));
    colors.Push(Convert::fromBullet(color));
#endif
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
    LogError( warningString );
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

int BulletDebugDrawer::getDebugMode() const
{
    return 1;
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif