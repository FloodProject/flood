/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"


namespace vapor { namespace scene {

//-----------------------------------//

class Node;

TYPEDEF_SHARED_POINTER_FROM_CLASS( Node );
TYPEDEF_SHARED_WEAK_POINTER_FROM_CLASS( Node );

//-----------------------------------//

/**
 * Represents a component, a specific piece of functionality that can be
 * added to each entity in the scene. A component will tipically hold a
 * piece of data and update it's state each frame if it needs to do it.
 * A component will also be able to register methods for scripting.
 */

class VAPOR_API Component : private boost::noncopyable
{
public:

	explicit Component();
	virtual ~Component();
	
	// Called once per frame to update the component.
	virtual void update( double delta ) = 0;

	// Gets the type of this component. 
	// Each component should have a unique type string.
	virtual const std::string& getType() const = 0;

	virtual void appendDebugGeometry( /*render::RenderQueue& queue*/ );

	// Gets the associated node of this component.
	NodePtr getNode() const;

	// Sets the associated node of this component.
	void setNode( NodePtr node );

protected:

	NodeWeakPtr node;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Component );

//-----------------------------------//

} } // end namespaces