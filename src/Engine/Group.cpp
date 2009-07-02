/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/scene/Group.h"

#include <sstream>

using namespace std;

namespace vapor {
	namespace scene {

//-----------------------------------//

int Group::add(shared_ptr<Node> child)
{
	shared_ptr<Node> ptr(this);
	child->setParent(ptr);
	children.push_back(child);
	return children.size()-1;
}

//-----------------------------------//

int Group::insert( int i, shared_ptr<Node> child )
{
	return 1;
}

//-----------------------------------//

bool Group::remove( int i )
{
	return false;
}

//-----------------------------------//

string Group::save(int ind)
{
	ostringstream os; 
	string space(ind, ' ');

	os << space << "\"" << name() << "\": {\n";
	os << space << "  " << "\"nodes\": [\n";

	// do all nodes
	std::vector<shared_ptr<Node>>::iterator it;
	for(it = children.begin(); it != children.end(); it++)
		os << (*it)->save(ind+4);

	os << space << "  " << "]\n";
	os << space << "}\n";
	return os.str();
}

//-----------------------------------//

} } // end namespaces