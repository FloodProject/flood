/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/ReferenceCount.h"
#include "Math/BoundingBox.h"
#include "Math/Frustum.h"
#include "Render/Renderable.h"

FWD_DECL_INTRUSIVE(Entity)
FWD_DECL_INTRUSIVE(Renderable)

namespace vapor {

//-----------------------------------//

//class VAPOR_API DebugRenderable : public Renderable
//{
//public:
//
//	DebugRenderable();
//
//	// Gets if the debug renderable is visible.
//	virtual bool isVisible() const;
//	
//	// Sets the debug renderable as visible.
//	virtual void setVisible( bool visible );
//
//	// Gets/sets if the debug renderable inherits the transform.
//	ACESSOR(InheritsTransform, bool, inheritsTransform)
//
//protected:
//
//	// Is the debug representation visible.
//	bool visible;
//
//	// Does the debug inherit the transform.
//	bool inheritsTransform;
//};
//
//TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( DebugRenderable );

//-----------------------------------//

// Builds debug geometry of a bounding box.
RenderablePtr VAPOR_API buildBoundingRenderable( const BoundingBox& box );

// Builds debug geometry of a ray.
EntityPtr VAPOR_API buildRay( const Ray& pickRay, const Vector3& outFar );

// Builds debug geometry of a frustum.
RenderablePtr VAPOR_API buildFrustum( const Frustum& box );

// Updates the debug geometry of a frustum.
void VAPOR_API updateDebugFrustum( const RenderablePtr& rend, const Frustum& box );

//-----------------------------------//

} // end namespace