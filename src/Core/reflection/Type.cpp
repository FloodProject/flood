/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/core/Type.h"

namespace vapor {

//-----------------------------------//

Type::Type(MetaType::Enum type, const std::string& name)
	: metaType(type)
	, name(name)
	, parent(nullptr)
{ }

//-----------------------------------//

Type::Type(MetaType::Enum type, const std::string& name, const Type& _parent)
	: metaType(type)
	, name(name)
	, parent(&_parent)
{ }

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

} // end namespace