/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "core/Class.h"
#include "core/Primitive.h"
#include "math/Color.h"

namespace vapor {

//-----------------------------------//

Class::Class(const std::string& name, int size)
	: Type(MetaType::Class, name, size)
{ }

//-----------------------------------//

Class::Class(const std::string& name, const Type& parent, int size)
	: Type(MetaType::Class, name, parent, size)
{ }

//-----------------------------------//

void Class::addField(Field& field)
{
	fields[field.name] = &field;
	fieldsVector.push_back(&field);
}

//-----------------------------------//

const FieldsMap& Class::getFields() const
{
	return fields;
}

//-----------------------------------//

const std::vector<Field*>& Class::getFieldsVector() const
{
	return fieldsVector;
}

//-----------------------------------//

Field* Class::getField(const std::string& name) const
{
	FieldsMap::const_iterator it = fields.find(name);

	if( it != fields.end() )
		return it->second;

	if( !parent ) return nullptr;

	Class* parentClass = (Class*) parent;
	return parentClass->getField(name);
}

//-----------------------------------//

void* Class::createInstance() const
{
	return nullptr;
}

//-----------------------------------//

//void* Class::doClone(const Class& type, void* object)
//{
//	void* newObject = type.createInstance();
//	const FieldsMap& fields = type.getFields();
//	
//	FieldsMap::const_iterator it;
//	for( it = fields.cbegin(); it != fields.cend(); it++ )
//	{
//		Field* field = it->second;
//		
//		//if( field->isPointer() )
//		//{
//		//	field->set<ResourcePtr>(newObject, field->get<ResourcePtr>(object));
//		//	continue;
//		//}
//
//		if( !field->type.isPrimitive() )
//			continue;
//		
//		const Primitive& ptype = (Primitive&) field->type;
//
//		if( ptype.isBool() )
//		{
//			field->set<bool>(newObject, field->get<bool>(object));
//		}
//		//-----------------------------------//
//		else if( ptype.isInteger() )
//		{
//			field->set<int>(newObject, field->get<int>(object));
//		}
//		//-----------------------------------//
//		else if( ptype.isFloat() )
//		{
//			field->set<float>(newObject, field->get<float>(object));
//		}
//		//-----------------------------------//
//		else if( ptype.isString() )
//		{
//			field->set<std::string>(newObject, field->get<std::string>(object));
//		}
//		//-----------------------------------//
//		else if( ptype.isColor() )
//		{
//			field->set<Color>(newObject, field->get<Color>(object));
//		}
//		//-----------------------------------//
//		else if( ptype.isVector3() )
//		{
//			field->set<Vector3>(newObject, field->get<Vector3>(object));
//		}
//	}
//
//	return newObject;
//}

//-----------------------------------//

void Class::notifyChanged(const Field& field) const
{
	onFieldChanged(field);
}

//-----------------------------------//

} // end namespace