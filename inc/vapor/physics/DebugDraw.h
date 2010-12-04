/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_PHYSICS_BULLET

#include "ReferenceCount.h"

FWD_DECL_INTRUSIVE(VertexBuffer)
FWD_DECL_INTRUSIVE(Material)
FWD_DECL_INTRUSIVE(Renderable)

#include <LinearMath/btIDebugDraw.h>

namespace vapor {

//-----------------------------------//

class BulletDebugDrawer : public btIDebugDraw
{
public:

	BulletDebugDrawer();

	// Gets the renderable.
	GETTER(Renderable, RenderablePtr, renderable)

	// Gets the dynamic vertex buffer.
	GETTER(VertexBuffer, VertexBufferPtr, vb)

	// Clears the dynamic vertex buffer.
	void clearBuffer();

	// Draws a line.
	void drawLine(const btVector3 &from, const btVector3 &to,
		const btVector3 &color);

	// Draws a contact point.
	virtual void drawContactPoint(const btVector3 &PointOnB,
		const btVector3 &normalOnB,	btScalar distance,
		int lifeTime, const btVector3 &color);

	// Reports error messages.
	virtual void reportErrorWarning (const char *warningString);
	
	// Draws 3D text.
	virtual void draw3dText (const btVector3 &location, const char *textString);
	
	// Gets the debug mode.
	virtual int	getDebugMode() const;

	// Sets the debug mode.
	virtual void setDebugMode(int debugMode);

protected:
	
	VertexBufferPtr vb;
	MaterialPtr material;
	RenderablePtr renderable;
};

//-----------------------------------//

} // end namespace

#endif