/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Reflection.h"
#include "ReferenceCount.h"

FWD_DECL_INTRUSIVE(Renderable)

namespace vapor {

//-----------------------------------//

class Entity;

TYPEDEF_SHARED_POINTER_FROM_TYPE( Entity )
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Entity )

//-----------------------------------//

/**
 * Represents a component, a specific piece of functionality that can be
 * added to each entity in the scene. A component will tipically hold a
 * piece of data and update it's state each frame if it needs to do it.
 * A component will also be able to register methods for scripting.
 */

class VAPOR_API Component
{
	DECLARE_CLASS_()
	DECLARE_UNCOPYABLE(Component)

public:

	explicit Component();
	virtual ~Component();
	
	// Gets the associated node of this component.
	EntityPtr getEntity() const;

	// Sets the associated node of this component.
	void setEntity( const EntityPtr& node );

	// Called once per frame to update the component.
	virtual void update( double delta ) = 0;

	// Gets if the debug renderable is visible.
	virtual bool isDebugRenderableVisible() const;
	
	// Sets the debug renderable as visible.
	virtual void setDebugRenderableVisible( bool visible );
	
	// Gets the debug renderable of this component.
	virtual RenderablePtr getDebugRenderable() const;

	// Creates the debug renderable of this component.
	virtual RenderablePtr createDebugRenderable() const;

	// Gets/sets if the debug renderable inherits the transform.
	ACESSOR(DebugInheritsTransform, bool, debugInheritsTransform)

protected:

	// Entity that owns this component.
	EntityWeakPtr node;

	// Is the debug representation visible.
	bool debugVisible;

	// Does the debug inherit the transform.
	mutable bool debugInheritsTransform;

	// Debug renderable of component.
	mutable RenderablePtr debugRenderable;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Component )
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Component )

//-----------------------------------//

} // end namespace