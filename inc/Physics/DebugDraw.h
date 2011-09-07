/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_PHYSICS_BULLET

#include "Core/References.h"
#include "Resources/Material.h"

FWD_DECL_INTRUSIVE(VertexBuffer)
FWD_DECL_INTRUSIVE(Material)
FWD_DECL_INTRUSIVE(Renderable)

#include <LinearMath/btIDebugDraw.h>

NAMESPACE_ENGINE_BEGIN

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
	void drawContactPoint(const btVector3 &PointOnB,
		const btVector3 &normalOnB,	btScalar distance,
		int lifeTime, const btVector3 &color) OVERRIDE;

	// Reports error messages.
	void reportErrorWarning (const char *warningString) OVERRIDE;
	
	// Draws 3D text.
	void draw3dText (const btVector3 &location, const char *textString) OVERRIDE;
	
	// Gets the debug mode.
	int	getDebugMode() const OVERRIDE;

	// Sets the debug mode.
	void setDebugMode(int debugMode) OVERRIDE;

protected:
	
	VertexBufferPtr vb;
	MaterialHandle material;
	RenderablePtr renderable;
};

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif