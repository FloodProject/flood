/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Group.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Group, Entity)
END_CLASS()

//-----------------------------------//

Group::Group()
{ }

//-----------------------------------//

Group::Group( const std::string& name )
	: Entity(name)
{ }

//-----------------------------------//

void Group::add( const EntityPtr& node )
{
	// Beware that you have to assign a new Group-derived object
	// to a shared_ptr, else shared_from_this will return bad_weak_ptr.

	node->setParent( Entity::shared_from_this() );
	nodes.push_back( node );

	onEntityAdded(node);
	onChanged();
}

//-----------------------------------//

EntityPtr Group::findEntity( const std::string& name ) const
{
	for( uint i = 0; i < nodes.size(); i++ )
	{
		const EntityPtr& entity = nodes[i];
		
		if( entity->getName() == name )
			return entity;
	}

	return EntityPtr();
}

//-----------------------------------//

bool Group::remove( const EntityPtr& node )
{
	std::vector<EntityPtr>::iterator it =
		std::find(nodes.begin(), nodes.end(), node );

	if( it == nodes.end() )
		return false;

	onEntityRemoved(node);
	onChanged();

	nodes.erase(it);

	return true;
}

//-----------------------------------//

void Group::update( double delta )
{
	for( uint i = 0; i < nodes.size(); i++ )
	{
		const EntityPtr& node = nodes[i];
		node->update( delta );
	}
}

//-----------------------------------//

} // end namespace