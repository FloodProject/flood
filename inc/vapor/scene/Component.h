/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Renderable.h"
#include "vapor/Serialization.h"

namespace vapor { namespace scene {

//-----------------------------------//

class Node;

TYPEDEF_SHARED_POINTER_FROM_TYPE( Node );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Node );

//-----------------------------------//

#define DECLARE_COMPONENT_TYPE				\
	static const std::string& id;

#define IMPLEMENT_COMPONENT_TYPE(type,str)	\
	const std::string& type::id = str;

//-----------------------------------//

/**
 * Represents a component, a specific piece of functionality that can be
 * added to each entity in the scene. A component will tipically hold a
 * piece of data and update it's state each frame if it needs to do it.
 * A component will also be able to register methods for scripting.
 */

class VAPOR_API Component : public Serializable, private boost::noncopyable
{
public:

	explicit Component();
	virtual ~Component();
	
	// Gets the associated node of this component.
	NodePtr getNode() const;

	// Sets the associated node of this component.
	void setNode( const NodePtr& node );

	// Called once per frame to update the component.
	virtual void update( double delta ) = 0;

	// Gets if the debug renderable is visible.
	virtual bool isDebugRenderableVisible() const;
	
	// Sets the debug renderable as visible.
	virtual void setDebugRenderableVisible( bool draw );
	
	// Gets the debug renderable of this component.
	virtual render::RenderablePtr getDebugRenderable() const;

	// Gets the type of this component. 
	// Each component should have a unique type string.
	virtual const std::string& getType() const = 0;

	DECLARE_SERIALIZABLE();

protected:

	NodeWeakPtr node;
	bool drawDebugRenderable;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Component );

//-----------------------------------//

} } // end namespaces