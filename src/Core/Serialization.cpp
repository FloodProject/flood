/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_SERIALIZATION

#include "Core/Serialization.h"
#include "Core/Reflection.h"
#include "Core/Log.h"
#include "Core/ReferenceCount.h"

NAMESPACE_BEGIN

//-----------------------------------//

ObjectWalker::ObjectWalker(ReflectionVisitor& visitor)
	: v(visitor)
{ }

//-----------------------------------//

void ObjectWalker::process(const Object* object)
{
	const Class& type = (Class&) object->getType();

	ObjectData data;
	data.type = (Type*) &type;
	data.instance = (void*) object;

	processObject(data);
}

//-----------------------------------//

void ObjectWalker::processObject(ObjectData object)
{
	v.processBegin(object);
	processType(object);
	v.processEnd(object);
}

//-----------------------------------//

void ObjectWalker::processType(ObjectData object)
{
	if( !object.instance )
		return;

	const Type& type = *object.type;

	if( type.isClass() || type.isStruct() )
	{
		processClass(object);		
	}
	else if( type.isPrimitive() )
	{
		processPrimitive(object);
	}
	else if( type.isEnum() )
	{
		processEnum(object);
	}
	else assert( false );
}

//-----------------------------------//

void ObjectWalker::processClass(ObjectData object, bool parent)
{
	Class& type = (Class&) *object.type;
	v.processClassBegin(type, parent);

	if( type.parent )
	{
		ObjectData parent;
		parent.instance = object.instance;
		parent.type = (Type*) type.parent;
		
		processClass(parent, true);
	}

	std::vector<Field*>::const_iterator it;

	for( it = type.fields.begin(); it != type.fields.end(); it++ )
	{
		processField(object, **it);
	}

	v.processClassEnd(type, parent);
}

//-----------------------------------//

void ObjectWalker::processEnum(ObjectData object)
{
	const Enum& metaenum = (Enum&) *object.type;
	
	v.processEnumBegin(metaenum);

	int value = *(int*) object.instance;
	const String& name = metaenum.getName(value);
	
	v.processEnumElement(value, name);

	v.processEnumEnd(metaenum);
}

//-----------------------------------//

void ObjectWalker::processField(ObjectData object, const Field& field)
{
	if( !object.instance ) return;
	
	const Type& type = field.type;

	object.instance = (byte*) object.instance + field.offset;
	object.type = (Type*) &type;

	v.processFieldBegin(field);

	if( field.qualifiers & Qualifier::Array )
	{
		processArray(object, field);
	}
	else
	{
		if( field.isPointer() )
			processPointer(object, field);

		processType(object);
	}

	v.processFieldEnd(field);
}

//-----------------------------------//

void ObjectWalker::processPointer(ObjectData& object, const Field& field)
{
	void* address = object.instance;

	if( field.qualifiers & Qualifier::SharedPointer )
	{
		std::shared_ptr<Object>* shared = (std::shared_ptr<Object>*) address;
		address = shared->get();
	}
	else if( field.qualifiers & Qualifier::RefPointer )
	{
		RefPtr<Object>* ref = (RefPtr<Object>*) address;
		address = ref->get();
	}

	object.instance = address;

	if( !address ) return;

	const Object* elemObject = (Object*) address;
	object.type = (Type*) &elemObject->getType();
}

//-----------------------------------//

void ObjectWalker::processArray(ObjectData object, const Field& field)
{
	std::vector<byte>& array = * (std::vector<byte>*) object.instance;

	int size = 0;

	if( field.isPointer() )
		size = field.pointerSize;
	else
		size = field.size;

	uint numElems = array.size() / size;

	v.processArrayBegin(field.type, numElems);
		
	for(size_t i = 0; i < numElems; i++)
	{
		void* address = (&array.front() + i * size);

		ObjectData elem;
		elem.instance = address;
		elem.type = object.type;

		if( field.isPointer() )
			processPointer(elem, field);

		v.processArrayElementBegin(i);
		processType(elem);
		v.processArrayElementEnd(i);
	}

	v.processArrayEnd(field.type);
}

//-----------------------------------//

void ObjectWalker::processPrimitive(const ObjectData& object)
{
	const Primitive& type = (const Primitive&) *object.type;

	if( type.isBool() )
	{
		bool* val = (bool*) object.instance;
		v.processBool(type, *val);
	}
	//-----------------------------------//
	else if( type.isInteger() )
	{
		int* val = (int*) object.instance;
		v.processInt(type, *val);
	}
	//-----------------------------------//
	else if( type.isFloat() )
	{
		float* val = (float*) object.instance;
		v.processFloat(type, *val);
	}
	//-----------------------------------//
	else if( type.isString() )
	{
		String* val = (String*) object.instance;
		v.processString(type, *val);
	}
	//-----------------------------------//
	else if( type.isColor() )
	{
		Color* val = (Color*) object.instance;
		v.processColor(type, *val);
	}
	//-----------------------------------//
	else if( type.isVector3() )
	{
		Vector3* vec = (Vector3*) object.instance;
		v.processVector3(type, *vec);
	}
	//-----------------------------------//
	else if( type.isQuaternion() )
	{
		Quaternion* quat = (Quaternion*) object.instance;
		v.processQuaternion(type, *quat);
	}
	//-----------------------------------//
	else if( type.isBitfield() )
	{
		uint* bits = (uint*) object.instance;
		v.processBitfield(type, *bits);
	}
	else assert( false );
}

//-----------------------------------//

NAMESPACE_END

#endif