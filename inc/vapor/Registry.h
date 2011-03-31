/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once
#include <map>

namespace vapor {

//-----------------------------------//

class Type;
typedef API_CORE std::map<String, const Type*> TypeRegistryMap;

/**
 * Stores a globally acessible mapping of type names and their instances.
 * This can be useful when you need to get information about a type just
 * by its name. Things like serialization need this information.
 */

class API_CORE Registry
{
public:

	// Gets a type given a name.
	const Type* getType(const String& type);

	// Registers a new type.
	void registerType(const Type* type);

	// Maps all the type names to their type instances.
	TypeRegistryMap types;
};

//-----------------------------------//

} // end namespace
