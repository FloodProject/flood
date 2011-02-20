/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_SERIALIZATION_JSON

#include "Serialization.h"
#include "Stream.h"

#include <json/json.h>
namespace Json { class Value; }

namespace vapor {

//-----------------------------------//

/**
 * Serializer implementation using JSON.
 */

class CORE_API JsonSerializer : public ReflectionVisitor
{
	DECLARE_UNCOPYABLE(JsonSerializer)

public:

	JsonSerializer(Stream& stream);

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
	virtual void processEnumElement(int value, const std::string& name);

	// Processes an array type.
	virtual void processArrayBegin(const Type& type, int size);
	virtual void processArrayEnd(const Type& type);
	virtual void processArrayElementBegin(int i);
	virtual void processArrayElementEnd(int i);

	// Processes a primitive type.
	virtual void processBool( const Primitive&, bool );
	virtual void processInt( const Primitive&, int );
	virtual void processUInt( const Primitive&, uint );
	virtual void processFloat( const Primitive&, float );
	virtual void processDouble( const Primitive&, double );
	virtual void processString( const Primitive&, const std::string& );
	virtual void processVector3( const Primitive&, const Vector3& );
	virtual void processQuaternion( const Primitive&, const Quaternion& );
	virtual void processColor( const Primitive&, const Color& );
	virtual void processBitfield( const Primitive&, const uint& );

protected:

	// Stream.
	Stream& stream;

	// Root JSON value.
	Json::Value rootValue;

	// Stack of JSON values.
	std::stack<Json::Value*> values;
};

//-----------------------------------//

class CORE_API JsonDeserializer
{
	DECLARE_UNCOPYABLE(JsonDeserializer)

public:

	JsonDeserializer(Stream& stream);

	// Deserializes the stream into an object.
	Object* deserialize();

protected:

	Object* processObject(const Json::Value& value);
	void processFields(Object* object, const Json::Value& value);
	void processField(Object* object, Field& field, const Json::Value& value);
	void processEnum(Object* object, Field& field, const Json::Value& value);
	void processPrimitive(void* address, Field& field, const Json::Value& value);	
	void processArray(Object* object, Field& field, const Json::Value& value);	
	void processArrayElement(void* element, Field& field, const Json::Value& value);
	byte* processArrayPointer(Object* address, Field& field, int size);

	Stream& stream;
	Registry& registry;
};

//-----------------------------------//

} // end namespace

#endif