/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Geometry.h"
#include "Math/Frustum.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(Projector)

class RenderView;

class API_ENGINE Projector : public Geometry
{
	REFLECT_DECLARE_OBJECT(Projector)

public:

	Projector();
	virtual ~Projector();

	// Gets the camera frustum.
	Frustum& getFrustum() { return frustum; }

	// Updates this component.
	void update( float delta );

	// Appends all the renderables of this geometry to the queue.
	void appendRenderables( RenderQueue& queue, const TransformPtr& transform ) OVERRIDE;

	// Pre-render callback.
	void onPreRender(RenderView*, const RenderState& state);

	// Updates the debug renderable of the camera.
	void updateDebugRenderable() const;

	// Creates the debug renderable of the camera.
	RenderablePtr createDebugRenderable() const;

	// Projector frustum.
	Frustum frustum;

	// Projected material.
	MaterialHandle material;

	// Geometry to project unto.
	GeometryPtr geometry;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Projector );

//-----------------------------------//

NAMESPACE_ENGINE_END