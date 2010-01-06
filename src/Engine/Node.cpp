/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Node.h"

namespace vapor { namespace scene {

//-----------------------------------//

Node::Node( const std::string& name )
	: name( name ), isVisible( true )
{

}

//-----------------------------------//

Node::Node() : isVisible( true )
{

}


//-----------------------------------//

Node::~Node()
{

}

//-----------------------------------//

NodePtr Node::getParent( ) const
{
	return parent.lock();
}

//-----------------------------------//

void Node::setParent( NodePtr parent )
{
	this->parent = parent;
}

//-----------------------------------//

bool Node::getVisible( ) const
{
	return isVisible;
}

//-----------------------------------//

void Node::setVisible( bool visible )
{
	this->isVisible = visible;
}

//-----------------------------------//

bool Node::addComponent( ComponentPtr component )
{
	const std::string& type = component->getType();

	// Searches for a component with the same type.
	if( components.find( type ) != components.end() )
	{
		warn( "scene", "Component of type %s already exists in entity %s", type, getName() );
		return false;
	}

	// If it doesn't exists, we add a new one.
	components[type] = component;
	component->setNode( shared_from_this() );

	// Cache geometry (renderable) objects.
	GeometryPtr geometry = std::dynamic_pointer_cast< Geometry >( component );
	if( geometry ) geometries.push_back( geometry );

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

const std::map< std::string, ComponentPtr >& Node::getComponents()
{
	return components;
}

//-----------------------------------//

void Node::update( float delta )
{
	// Update Geometry components so bounding boxes are generated.
	foreach( ComponentPtr comp, getGeometry() )
		comp->update( delta );

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

Transform* Node::getTransformPtr()
{
	return getTransform().get();
}


//-----------------------------------//

const std::vector< GeometryPtr >& Node::getGeometry()
{
	return geometries;
}

//-----------------------------------//

const std::string& Node::getName() const
{
	return name;
}

//-----------------------------------//

void Node::setName( const std::string& name )
{
	this->name = name;
}

//-----------------------------------//

} } // end namespaces