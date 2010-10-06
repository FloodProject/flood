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
	FIELD_PRIMITIVE(Node, Bitfield, tags)
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
	if( !component )
		return false;

	const Class* type = &component->getInstanceType();

	if( components.find(type) != components.end() )
	{
		warn( "scene",
			"Component of type '%s' already exists in node '%s'",
			type->getName().c_str(), getName().c_str() );
		return false;
	}

	// Cache geometry (renderable) objects.
	if( type->is<Geometry>() || type->inherits<Geometry>() )
	{
		const GeometryPtr& geometry = std::dynamic_pointer_cast<Geometry>( component );
		geometries.push_back( geometry );
	}

	// If it doesn't exist yet, add it in the map.
	components[type] = component;
	component->setNode( shared_from_this() );

	if( !onComponentAdded.empty() )
		onComponentAdded(component);

	return true;
}

//-----------------------------------//

bool Node::removeComponent( const ComponentPtr& component )
{
	if( !component )
		return false;
	
	const Class* type = &component->getInstanceType();

	ComponentMap::iterator it = components.find(type);
	
	if( it == components.end() )
		return false;

	components.erase(it);
	
	if( !type->inherits<Geometry>() )
		return true;

	std::vector<GeometryPtr>::iterator it2;
	it2 = std::find( geometries.begin(), geometries.end(), component );
	
	assert( it2 != geometries.end() );
	geometries.erase( it2 );

	if( !onComponentRemoved.empty() )
		onComponentRemoved(component);

	return true;
}

//-----------------------------------//

void Node::update( double delta )
{
	const TransformPtr& transform = getTransform();

	// Update all geometry bounding boxes first.
	foreach( const GeometryPtr& geom, getGeometry() )
		geom->update( delta );

	// Update transform (info may be needed by other components)
	if( transform )
		transform->update( delta );

	// Update everything else.
	foreach( const ComponentMapPair& component, components )
	{
		if( component.second == transform ) 
			continue;

		component.second->update( delta );
	}
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

NodePtr Node::getParent() const
{
	return parent.lock();
}

//-----------------------------------//

bool Node::isVisible() const
{
	return visible;
}

//-----------------------------------//

bool Node::getTag(int index)
{
	return tags[index];
}

//-----------------------------------//

void Node::setTag(int index, bool state)
{
	tags[index] = state;
}

//-----------------------------------//

} // end namespace