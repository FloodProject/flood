/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "core/Type.h"
#include "core/Class.h"

namespace vapor {

//-----------------------------------//

Type::Type(MetaType::Enum type, const std::string& name, int size)
	: type(type)
	, name(name)
	, parent(nullptr)
	, size(size)
{
	init();
}

//-----------------------------------//

Type::Type(MetaType::Enum type, const std::string& name, const Type& _parent, int size)
	: type(type)
	, name(name)
	, parent(&_parent)
	, size(size)
{
	init();
}

//-----------------------------------//

Type::~Type()
{ }

//-----------------------------------//

void Type::init()
{
	GetRegistry().registerType(this);

	if(!isClass() || !parent)
		return;

	Class* klass = (Class*) parent;
	klass->addChild((Class&) *this);
}

//-----------------------------------//

bool Type::isPrimitive() const
{
	return type == MetaType::Primitive;
}

//-----------------------------------//

bool Type::isStruct() const
{
	return type == MetaType::Structure;
}

//-----------------------------------//

bool Type::isClass() const
{
	return type == MetaType::Class;
}

//-----------------------------------//

bool Type::isEnum() const
{
	return type == MetaType::Enumeration;
}

//-----------------------------------//

Registry& Type::GetRegistry()
{
	static Registry registry;
	return registry;
}

//-----------------------------------//

void Registry::registerType(const Type* type)
{
	if(!type)
		return;

	types[type->name] = type;
}

//-----------------------------------//

const Type* Registry::getType(const std::string& type)
{
	if( types.find(type) == types.end() )
		return nullptr;

	return types[type];
}

//-----------------------------------//

} // end namespace