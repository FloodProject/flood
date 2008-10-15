#pragma once

#include "vapor/Resource.h"

namespace vapor {
	namespace resources {

class ResourceManager 
{

public:

	void loadResource();
	Resource& getResource();


private:

	//std::map<Resource&> resourceMap;


};



} } // end namespaces