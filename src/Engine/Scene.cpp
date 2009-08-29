/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Scene.h"

#include <sstream>

namespace vapor {
	namespace scene {

//-----------------------------------//

Scene::~Scene()
{

}

//-----------------------------------//

std::string Scene::save(int ind)
{
	std::ostringstream os;
	os << "{ " << Group::save(ind);
	return os.str();
}

//-----------------------------------//

} } // end namespaces