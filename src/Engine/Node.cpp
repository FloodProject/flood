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

BEGIN_CLASS(Node)
	FIELD_PRIMITIVE(Node, string, name)
	FIELD_PRIMITIVE(Node, bool, visible)
END_CLASS()

//-----------------------------------//

Node::Node( const std::string& name )
	: name( name )
	, visible( true )
{ }

//-----------------------------------//

Node::Node()
	: visible( true )
{ }

//-----------------------------------//

bool Node::addComponent( const ComponentPtr& component )
{
	assert( component != nullptr );

	const Class* type = &component->getInstanceType();

	// Searches for a component with the same type.
	if( components.find(type) != components.end() )
	{
		warn( "scene",
			"Component of type '%s' already exists in node '%s'",
			type->getName().c_str(), getName().c_str() );

		return false;
	}

	// Cache geometry (renderable) objects.
	if( type->inherits<Geometry>() )
	{
		const GeometryPtr& geometry =
			std::dynamic_pointer_cast<Geometry>( component );
	
		geometries.push_back( geometry );
	}

	// If it doesn't exist yet, add it in the map.
	components[type] = component;
	component->setNode( shared_from_this() );

	return true;
}

//-----------------------------------//

bool Node::removeComponent( const std::string& type )
{
	assert( false );

	//assert( !type.empty() );

	//if( type.empty() ) 
	//	return false;

	//// Searches for a component with the same type.
	//ComponentMap::iterator it = components.find( type );
	//if( it == components.end() )
	//	return false;
	//
	//const ComponentPtr& component = components[type];
	//components.erase( it );

	//// Check if we cached the component...
	//const GeometryPtr& geometry = std::dynamic_pointer_cast< Geometry >( component );
	//if( geometry ) 
	//{
	//	std::vector< GeometryPtr >::iterator it;
	//	it = std::find( geometries.begin(), geometries.end(), geometry );

	//	if( it != geometries.end() )
	//		geometries.erase( it );
	//}

	return true;
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
	if( transform )
		transform->update( delta );
}

//-----------------------------------//

bool Node::addTransform()
{
	TransformPtr transform( new Transform() );
	return addComponent(transform);
}

//-----------------------------------//

TransformPtr Node::getTransform() const
{
	return getComponent<Transform>();
}

//-----------------------------------//

NodePtr Node::getParent( ) const
{
	return parent.lock();
}

//-----------------------------------//

bool Node::isVisible( ) const
{
	return visible;
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

} // end namespace