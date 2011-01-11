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

class CORE_API JsonSerializer : public Serializer
{
	DECLARE_UNCOPYABLE(JsonSerializer)

public:

	JsonSerializer(Stream& stream);

	// Flushes the serialization to the stream.
	virtual void flushStream();

	// Begins a new class block.
	virtual void beginClass(const Class& type);

	// Ends the current class block.
	virtual void endClass();

	// Writes a field into the class block.
	virtual void writeField( const Field& field );

	// Begins a new array block.
	virtual void beginArray(const Type& type);

	// Ends the current array block.
	virtual void endArray();

	// Primitive Primitives.
	virtual void writeBool( const Primitive&, bool );
	virtual void writeInt( const Primitive&, int );
	virtual void writeUInt( const Primitive&, uint );
	virtual void writeFloat( const Primitive&, float );
	virtual void writeDouble( const Primitive&, double );
	virtual void writeString( const Primitive&, const std::string& );
	virtual void writeVector3( const Primitive&, const Vector3& );
	virtual void writeQuaternion( const Primitive&, const Quaternion& );
	virtual void writeColor( const Primitive&, const Color& );
	virtual void writeBitfield( const Primitive&, const std::bitset<32>& );
	//virtual void writeHandle( const Handle& );

protected:

	// Stream.
	Stream& stream;

	// Root JSON value.
	Json::Value rootValue;

	// Stack of JSON values.
	std::stack<Json::Value> values;
};

//-----------------------------------//

} // end namespace

#endif