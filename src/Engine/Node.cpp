/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Node.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

Node::Node()
	/*: parent( NodePtr )*/
{

}

//-----------------------------------//

Node::~Node()
{

}

//-----------------------------------//

NodePtr Node::getParent( ) const
{
	NodePtr ret( parent.lock() );
	return ret;
}

//-----------------------------------//

void Node::setParent( NodePtr parent )
{
	this->parent = parent;
}

//-----------------------------------//

bool Node::addComponent( ComponentPtr component )
{
	// Searches for a component with the same type.
	if( components.find( component->getType() ) != components.end() )
	{
		return false;
	}

	// If it doesn't exists, we add a new one.
	components[component->getType()] = component;
	component->setNode( shared_from_this() );

	// Cache renderable objects
	GeometryPtr geometry = std::dynamic_pointer_cast< Geometry >( component );

	if( geometry )
	{
		geometries.push_back( geometry );
	}

	return true;
}

//-----------------------------------//

ComponentPtr Node::getComponent( const std::string& type )
{
	if( components.find( type ) == components.end() )
	{
		return ComponentPtr();
	}

	return components[type];
}

//-----------------------------------//

void Node::update( float delta )
{
	foreach( componentPair component, components )
	{
		component.second->update( delta );
	}
}

//-----------------------------------//

TransformPtr Node::getTransform()
{
	return getComponent< Transform >( "Transform" );
}

//-----------------------------------//

const std::vector< GeometryPtr >& Node::getGeometry()
{
	return geometries;
}

//-----------------------------------//

const std::string Node::name() const
{
	return "Node";
}

//-----------------------------------//

} } // end namespaces