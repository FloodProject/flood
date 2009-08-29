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

int Group::add(shared_ptr<Node> child)
{
	child->setParent(this);
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

std::string Group::save(int ind)
{
	ostringstream os; 
	string space(ind, ' ');

	os << space << "\"" << name() << "\": {\n";
	os << space << "  " << "\"nodes\": [\n";

	// do all nodes
	foreach( shared_ptr<Node> node, children )
		os << node->save(ind+4);

	os << space << "  " << "]\n";
	os << space << "}\n";
	return os.str();
}

//-----------------------------------//

} } // end namespaces