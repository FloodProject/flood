/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Scene/Geometry.h"
#include "Core/Math/Frustum.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class RenderView;

API_SCENE REFLECT_DECLARE_CLASS(Projector)

class API_SCENE Projector : public Geometry
{
	REFLECT_DECLARE_OBJECT(Projector)

public:

	Projector();
	virtual ~Projector();

	// Gets the camera frustum.
	Frustum& getFrustum() { return frustum; }

	// Updates this component.
	void update( float delta );

	// Appends all the.renderables of this geometry to the queue.
	virtual void appendRenderables( RenderQueue& queue, const Transform* transform ) OVERRIDE;

	// Pre-render callback.
	void onPreRender(RenderView*, const RenderState& state);

	// Updates the debug.renderable of the camera.
	void updateDebugRenderable() const;

	// Creates the debug.renderable of the camera.
	RenderBatchPtr createDebuRenderable() const;

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