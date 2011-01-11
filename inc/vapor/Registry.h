/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

class Type;
typedef CORE_API std::map<std::string, const Type*> TypeRegistryMap;

/**
 * Stores a globally acessible mapping of type names and their instances.
 * This can be useful when you need to get information about a type just
 * by its name. Things like serialization need this information.
 */

class CORE_API Registry
{
public:

	// Registers a new type mapping.
	void registerType(const Type& type);

	// Gets a type given a name.
	const Type* getType(const std::string& type);

	// Maps all the type names to their type instances.
	TypeRegistryMap types;
};

//-----------------------------------//

} // end namespace
