/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Node.h"
#include "vapor/scene/Tags.h"

namespace vapor {

//-----------------------------------//

Node::Node( const std::string& name )
	: name( name ),
	_isVisible( true )
{ }

//-----------------------------------//

Node::Node()
	: _isVisible( true )
{ }

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
	const GeometryPtr& geometry = 
		std::dynamic_pointer_cast<Geometry>( component );
	
	if( geometry ) geometries.push_back( geometry );

	// If it doesn't exist yet, add it in the map.
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
	
	const ComponentPtr& component = components[type];
	components.erase( it );

	// Check if we cached the component...
	const GeometryPtr& geometry = std::dynamic_pointer_cast< Geometry >( component );
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

ComponentPtr Node::getComponent( const std::string& type ) const
{
	ComponentMap::const_iterator it = components.find(type);
	if( it == components.end() )
	{
		return ComponentPtr();
	}

	return (*it).second;
}

//-----------------------------------//

void Node::update( double delta )
{
	const TransformPtr& transform = getTransform();

	// Update all geometry bounding boxes first.
	foreach( const GeometryPtr& geom, getGeometry() )
		geom->update( delta );

	// Update everything else.
	foreach( const ComponentMapPair& component, components )
	{
		if( component.second == transform ) 
			continue;

		component.second->update( delta );
	}

	// Update transform (info may be needed by other components)
	if( transform ) transform->update( delta );
}

//-----------------------------------//

bool Node::addTransform()
{
	return addComponent(
		TransformPtr( new Transform() ) );
}

//-----------------------------------//

TransformPtr Node::getTransform() const
{
	return getComponent< Transform >( "Transform" );
}

//-----------------------------------//

NodePtr Node::getParent( ) const
{
	return parent.lock();
}

//-----------------------------------//

bool Node::isVisible( ) const
{
	return _isVisible;
}

//-----------------------------------//

bool Node::getTag( int index )
{
	return tag[index];
}

//-----------------------------------//

void Node::setTag( int index, bool state )
{
	tag[index] = state;
}

//-----------------------------------//

void Node::serialize( Json::Value& value )
{
	//value["name"] = getName();
	if( !isVisible() ) value["visible"] = isVisible();
	if( tag.any() ) value["tags"] = tag.to_string();

	foreach( const ComponentMapPair& pair, components )
	{
		const ComponentPtr& c = pair.second;
		c->serialize( value["components"][c->getType()] );
	}
}

//-----------------------------------//

} // end namespace