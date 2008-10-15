#pragma once

#include "vapor/Types.h"
#include "vapor/Resource.h"

namespace vapor {
	namespace resources {

class ResourceManager 
{

public:

	void loadResource();
	Resource& getResource(string &name);


private:

	//std::map<Resource&> resourceMap;


};



} } // end namespaces