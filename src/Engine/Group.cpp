/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Group.h"
#define VAPOR_GROUP_USE_FOREACH

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Group, Node)
END_CLASS()

//-----------------------------------//

Group::Group()
{ }

//-----------------------------------//

Group::Group( const std::string& name )
	: Node(name)
{ }

//-----------------------------------//

void Group::add( const NodePtr& node )
{
	// Beware that you have to assign a new Group-derived object
	// to a shared_ptr, else shared_from_this will return bad_weak_ptr.

	node->setParent( Node::shared_from_this() );
	nodes.push_back( node );

	onNodeAdded(node);
}

//-----------------------------------//

NodePtr Group::findNode( const std::string& name ) const
{
	foreach( const NodePtr& node, nodes )
	{
		if( node->getName() == name )
			return node;
	}

	return NodePtr();
}

//-----------------------------------//

bool Group::remove( const NodePtr& node )
{
	std::vector<NodePtr>::iterator it =
		std::find(nodes.begin(), nodes.end(), node );

	if( it == nodes.end() )
		return false;

	onNodeRemoved(node);
	nodes.erase(it);

	return true;
}

//-----------------------------------//

void Group::update( double delta )
{
#ifdef VAPOR_GROUP_USE_FOREACH
	foreach( const NodePtr& node, nodes )
	{
		node->update( delta );
	}
#else
	typedef std::vector<NodePtr>::const_iterator vec_it;
	vec_it it = nodes.begin();
	vec_it end = nodes.end();
	
	while( ++it != end )
	{
		(*it)->update( delta );
	}
#endif
}

//-----------------------------------//

} // end namespace