/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Group.h"

namespace vapor { namespace scene {

//-----------------------------------//

Group::Group( const std::string& name )
	: Node( name )
{

}

//-----------------------------------//

Group::~Group()
{

}

//-----------------------------------//

int Group::add( NodePtr child )
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

int Group::insert( int i, NodePtr child )
{
	// TODO
	return 1;
}

//-----------------------------------//

bool Group::remove( int i )
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

NodePtr Group::get( uint i ) const
{
	if( i >= children.size() )
	{
		return NodePtr();
	}

	return children[i];
}

//-----------------------------------//

void Group::update( float delta )
{
	foreach( NodePtr node, children )
	{
		node->update( delta );
	}
}

//-----------------------------------//

int Group::count() const
{
	return children.size();
}

//-----------------------------------//

const std::string Group::save(int ind)
{
	//ostringstream os; 
	//string space(ind, ' ');

	//os << space << "\"" << name() << "\": {\n";
	//os << space << "  " << "\"nodes\": [\n";

	//// do all nodes
	////foreach( NodePtr node, children )
	////	os << node->save(ind+4);

	//os << space << "  " << "]\n";
	//os << space << "}\n";
	//return os.str();

	return "";
}

//-----------------------------------//

//std::shared_ptr< Group > Group::shared_from_this()
//{ 
//	return std::static_pointer_cast< Group >( 
//		Node::shared_from_this() ); 
//}

//-----------------------------------//

//std::shared_ptr< Group > Group::shared_from_this() const
//{ 
//	return std::static_pointer_cast< Group >( 
//		Node::shared_from_this() ); 
//}

//-----------------------------------//

} } // end namespaces