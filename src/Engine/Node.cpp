/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Node.h"

namespace vapor { namespace scene {

//-----------------------------------//

Node::Node( const std::string& name )
	: name( name ), _isVisible( true )
{
}

//-----------------------------------//

Node::Node() : _isVisible( true )
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

bool Node::isVisible( ) const
{
	return _isVisible;
}

//-----------------------------------//

void Node::setVisible( bool visible )
{
	_isVisible = visible;
}

//-----------------------------------//

bool Node::addComponent( const ComponentPtr& component )
{
	assert( component != nullptr );

	const std::string& type = component->getType();

	// Searches for a component with the same type.
	if( components.find( type ) != components.end() )
	{
		warn( "scene", "Component of type %s already exists in entity %s",
			type.c_str(), getName().c_str() );
		return false;
	}

	// Cache geometry (renderable) objects.
	GeometryPtr geometry = std::dynamic_pointer_cast< Geometry >( component );
	if( geometry ) geometries.push_back( geometry );

	// If it doesn't exists, we add a new one.
	components[type] = component;
	component->setNode( shared_from_this() );

	return true;
}

//-----------------------------------//

bool Node::removeComponent( const std::string& type )
{
	assert( !type.empty() );

	if( type.empty() ) return false;

	// Searches for a component with the same type.
	ComponentMap::iterator it = components.find( type );
	if( it == components.end() )
		return false;
	
	ComponentPtr component = components[type];
	components.erase( it );

	// Check if we cached the component...
	GeometryPtr geometry = std::dynamic_pointer_cast< Geometry >( component );
	if( geometry ) 
	{
		std::vector< GeometryPtr >::iterator it;
		it = std::find( geometries.begin(), geometries.end(), geometry );

		if( it != geometries.end() )
			geometries.erase( it );

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

const ComponentMap& Node::getComponents()
{
	return components;
}

//-----------------------------------//

void Node::update( double delta )
{
	const TransformPtr& transform = getTransform();

	// Update all geometry bounding boxes first
	foreach( const GeometryPtr& geom, getGeometry() )
		geom->update( delta );

	// Update transform (info may be needed by other components)
	if( transform ) transform->update( delta );

	// Update everything else
	foreach( const ComponentMapPair& component, components )
	{
		if( component.second == transform ) 
			continue;

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