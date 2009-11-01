/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

class Node;

TYPEDEF_SHARED_POINTER_FROM_CLASS( Node );

//-----------------------------------//

/**
 * Represents a component, a specific piece of functionality that can be
 * added to each entity in the scene. A component will tipically hold a
 * piece of data and update it's state each frame if it needs to do it.
 * A component will also be able to register methods for scripting.
 */

class VAPOR_API Component :	private boost::noncopyable
{
public:

	explicit Component();
	virtual ~Component();
	
	// Called once per frame to update the component.
	virtual void update( float delta ) = 0;

	// Gets the type of this component. 
	// Each component should have a unique type string.
	virtual const std::string& getType() const = 0;

	// Sets the associated node of this component.
	void setNode( NodePtr node );

	// Gets the associated node of this component.
	NodePtr getNode() const;

protected:

	//static std::string type;
	std::weak_ptr< Node > node;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Component );

//-----------------------------------//

} } // end namespaces