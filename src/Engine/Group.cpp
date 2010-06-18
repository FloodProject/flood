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

Group::Group( const std::string& name )
	: Node( name )
{ }

//-----------------------------------//

int Group::add( const NodePtr& child )
{
	// Beware that you have to assign a new Group-derived object
	// to a shared_ptr, else shared_from_this will return bad_weak_ptr.

	child->setParent( Node::shared_from_this() );
	children.push_back( child );

	if( !onNodeAdded.empty() )
	{
		GroupEvent event;
		event.node = child;

		onNodeAdded( event );
	}
	
	return children.size() - 1;
}

//-----------------------------------//

int Group::insert( uint i, const NodePtr& child )
{
	// TODO
	return 1;
}

//-----------------------------------//

bool Group::remove( uint i )
{
	// TODO

	//if( !onNodeRemoved.empty() )
	//{
	//	GroupEvent event;
	//	event.node = child;

	//	onNodeRemoved( event );
	//}

	return false;
}

//-----------------------------------//

bool Group::remove( const NodePtr& node )
{
	NodeVector::iterator it =
		std::find(children.begin(), children.end(), node );

	if( it == children.end() )
		return false;

	children.erase(it);

	if( !onNodeRemoved.empty() )
	{
		GroupEvent event;
		event.node = node;

		onNodeRemoved( event );
	}

	return true;
}

//-----------------------------------//

int Group::index( const NodePtr& child ) const
{
	NodeVector::const_iterator it;
	it = std::find( children.begin(), children.end(), child );

	if( it == children.end() )
		return -1;

	return std::distance(children.begin(), it);
}

//-----------------------------------//

NodePtr Group::get( uint i ) const
{
	if( i >= children.size() )
	{
		return NodePtr();
	}

	return children[i];
}

//-----------------------------------//

void Group::update( double delta )
{
#ifdef VAPOR_GROUP_USE_FOREACH
	foreach( const NodePtr& node, children )
	{
		node->update( delta );
	}
#else
	typedef std::vector<NodePtr>::const_iterator vec_it;
	vec_it it = children.begin();
	vec_it end = children.end();
	
	while( ++it != end )
	{
		(*it)->update( delta );
	}
#endif
}

//-----------------------------------//

int Group::count() const
{
	return children.size();
}

//-----------------------------------//

void Group::serialize( Json::Value& value )
{
	value["name"] = getName();

	foreach( const NodePtr& node, children )
		node->serialize( value["nodes"][node->getName()] );
}

//-----------------------------------//

} // end namespace