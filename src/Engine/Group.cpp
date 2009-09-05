/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Group.h"

#include <sstream>

using namespace std;

namespace vapor {
	namespace scene {

//-----------------------------------//

Group::~Group()
{

}

//-----------------------------------//

int Group::add( NodePtr child )
{
	child->setParent(this);
	children.push_back(child);
	return children.size()-1;
}

//-----------------------------------//

int Group::insert( int i, NodePtr child )
{
	return 1;
}

//-----------------------------------//

bool Group::remove( int i )
{
	return false;
}

//-----------------------------------//

void Group::update()
{
	foreach( NodePtr node, children )
	{
		node->update();
	}
}

//-----------------------------------//

int Group::count() const
{
	return children.size();
}

//-----------------------------------//

std::string Group::save(int ind)
{
	ostringstream os; 
	string space(ind, ' ');

	os << space << "\"" << name() << "\": {\n";
	os << space << "  " << "\"nodes\": [\n";

	// do all nodes
	foreach( NodePtr node, children )
		os << node->save(ind+4);

	os << space << "  " << "]\n";
	os << space << "}\n";
	return os.str();
}

//-----------------------------------//

} } // end namespaces