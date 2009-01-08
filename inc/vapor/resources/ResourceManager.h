/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Engine.h"
#include "vapor/resources/Resource.h"

#include <map>

namespace vapor {
	namespace resources {

class ResourceManager 
{

public:
	ResourceManager();
	virtual ~ResourceManager();

	void addResource(const char* name, Resource* resource);
	Resource* getResource(string &name);

private:

	map<string, Resource*> resourceMap;
};



} } // end namespaces
