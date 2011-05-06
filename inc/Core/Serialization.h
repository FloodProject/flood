/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Reflection.h"
#include "Core/Object.h"

#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Math/EulerAngles.h"
#include "Math/Color.h"

NAMESPACE_BEGIN

//-----------------------------------//

struct API_CORE ObjectData
{
	void* instance;
	Type* type;
};

//-----------------------------------//

class API_CORE ReflectionVisitor
{
public:

	// Processes a type.
	virtual void processBegin(const ObjectData& data)  {}
	virtual void processEnd(const ObjectData& data)  {}

	// Processes a class type.
	virtual void processClassBegin(const Class& type, bool parent)  {}
	virtual void processClassEnd(const Class& type, bool parent)  {}
	
	// Processes class fields.
	virtual void processFieldBegin(const Field& field)  {}
	virtual void processFieldEnd(const Field& field)  {}

	// Processes an enum type.
	virtual void processEnumBegin( const Enum& enuhm )  {}
	virtual void processEnumEnd( const Enum& enuhm )  {}
	virtual void processEnumElement(int value, const String& name)  {}

	// Processes an array type.
	virtual void processArrayBegin(const Type& type, int size)  {}
	virtual void processArrayEnd(const Type& type)  {}
	virtual void processArrayElementBegin(int i)  {}
	virtual void processArrayElementEnd(int i)  {}

	// Processes a primitive type.
	virtual void processBool( const Primitive&, bool )  {}
	virtual void processInt( const Primitive&, int32 )  {}
	virtual void processUInt( const Primitive&, uint32 )  {}
	virtual void processFloat( const Primitive&, float )  {}
	virtual void processString( const Primitive&, const String& )  {}
	virtual void processVector3( const Primitive&, const Vector3& )  {}
	virtual void processQuaternion( const Primitive&, const Quaternion& )  {}
	virtual void processColor( const Primitive&, const Color& )  {}
	virtual void processBitfield( const Primitive&, const uint32& )  {}
};

//-----------------------------------//

class API_CORE ObjectWalker
{
	DECLARE_UNCOPYABLE(ObjectWalker)

public:

	ObjectWalker(ReflectionVisitor& visitor);

	// Processes an object.
	void process(const Object* object);

protected:

	// Processes an object.
	void processObject(ObjectData object);

	// Processes a type.
	void processType(ObjectData object);

	// Processes a class.
	void processClass(ObjectData object, bool parent = false);

	// Processes an enum.
	void processEnum(ObjectData object);

	// Processes a field of the object.
	void processField(ObjectData object, const Field& field);

	// Processes an array of the object.
	void processArray(ObjectData object, const Field& field);

	// Processes a primitive field of the object.
	void processPrimitive(const ObjectData& object);

	// Processes a pointer type.
	void processPointer(ObjectData& object, const Field& field);

protected:

	ReflectionVisitor& v;
};

//-----------------------------------//

NAMESPACE_END