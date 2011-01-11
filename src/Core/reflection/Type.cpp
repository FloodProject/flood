/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "core/Type.h"

namespace vapor {

//-----------------------------------//

Registry& Type::GetRegistry()
{
	static Registry registry;
	return registry;
}

//-----------------------------------//

Type::Type(MetaType::Enum type, const std::string& name, int size)
	: metaType(type)
	, name(name)
	, parent(nullptr)
	, size(size)
{
	registerInstance();
}

//-----------------------------------//

Type::Type(MetaType::Enum type, const std::string& name, const Type& _parent, int size)
	: metaType(type)
	, name(name)
	, parent(&_parent)
	, size(size)
{
	registerInstance();
}

//-----------------------------------//

Type::~Type()
{ }

//-----------------------------------//

void Type::registerInstance()
{
	GetRegistry().registerType(*this);
}

//-----------------------------------//

const std::string& Type::getName() const
{
	return name;
}

//-----------------------------------//

const Type* Type::getParent() const
{
	return parent;
}

//-----------------------------------//

bool Type::isPrimitive() const
{
	return metaType == MetaType::Primitive;
}

//-----------------------------------//

bool Type::isStruct() const
{
	return metaType == MetaType::Struct;
}

//-----------------------------------//

bool Type::isClass() const
{
	return metaType == MetaType::Class;
}

//-----------------------------------//

bool Type::isEnum() const
{
	return metaType == MetaType::Enumeration;
}

//-----------------------------------//

void Registry::registerType(const Type& type)
{
	const std::string& typeName = type.getName();
	types[typeName] = &type;
}

//-----------------------------------//

const Type* Registry::getType(const std::string& type)
{
	TypeRegistryMap::iterator it = types.find(type);

	if( it == types.end() )
		return nullptr;

	return types[type];
}

//-----------------------------------//

} // end namespace