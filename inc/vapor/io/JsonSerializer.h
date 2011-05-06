/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/API.h"

#ifdef ENABLE_SERIALIZATION_JSON

#include "Core/Serialization.h"
#include "Core/Stream.h"
#include <stack>

struct json_t;

NAMESPACE_BEGIN

//-----------------------------------//

/**
 * Serializer implementation using JSON.
 */

class API_CORE JsonSerializer : public ReflectionVisitor
{
	DECLARE_UNCOPYABLE(JsonSerializer)

public:

	JsonSerializer(Stream* stream);

	// Processes an object.
	virtual void processBegin(const ObjectData& data);
	virtual void processEnd(const ObjectData& data);

	// Processes a class type.
	virtual void processClassBegin(const Class& type, bool parent);
	virtual void processClassEnd(const Class& type, bool parent);

	// Processes class fields.
	virtual void processFieldBegin(const Field& field);
	virtual void processFieldEnd(const Field& field);

	// Processes an enum type.
	virtual void processEnumBegin( const Enum& enuhm );
	virtual void processEnumEnd( const Enum& enuhm );
	virtual void processEnumElement(int value, const String& name);

	// Processes an array type.
	virtual void processArrayBegin(const Type& type, int size);
	virtual void processArrayEnd(const Type& type);
	virtual void processArrayElementBegin(int i);
	virtual void processArrayElementEnd(int i);

	// Processes a primitive type.
	virtual void processBool( const Primitive&, bool );
	virtual void processInt( const Primitive&, int32 );
	virtual void processUInt( const Primitive&, uint32 );
	virtual void processFloat( const Primitive&, float );
	virtual void processString( const Primitive&, const String& );
	virtual void processVector3( const Primitive&, const Vector3& );
	virtual void processQuaternion( const Primitive&, const Quaternion& );
	virtual void processColor( const Primitive&, const Color& );
	virtual void processBitfield( const Primitive&, const uint32& );

protected:

	// Stream.
	Stream* stream;

	// Root JSON value.
	json_t* rootValue;

	// Stack of JSON values.
	std::stack<json_t*> values;
};

//-----------------------------------//

class API_CORE JsonDeserializer
{
	DECLARE_UNCOPYABLE(JsonDeserializer)

public:

	JsonDeserializer(Stream* stream);

	// Deserializes the stream into an object.
	Object* deserialize();

protected:

	Object* processObject(json_t* value);
	void processFields(Object* object, json_t* value);
	void processField(Object* object, Field& field, json_t* value);
	void processEnum(Object* object, Field& field, json_t* value);
	void processPrimitive(void* address, Field& field, json_t* value);	
	void processArray(Object* object, Field& field, json_t* value);	
	void processArrayElement(void* element, Field& field, json_t* value);
	byte* processArrayPointer(Object* address, Field& field, int size);

	Stream* stream;
};

//-----------------------------------//

NAMESPACE_END

#endif