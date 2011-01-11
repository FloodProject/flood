/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Reflection.h"
#include "math/Vector3.h"
#include "math/Quaternion.h"
#include "math/EulerAngles.h"
#include "math/Color.h"
#include <bitset>

namespace vapor {

//-----------------------------------//

/**
 * Abstract serialization API.
 */

class CORE_API Serializer
{
	DECLARE_UNCOPYABLE(Serializer)

public:

	Serializer() {}
	virtual ~Serializer() {}

	// Flushes the serialization to the stream.
	virtual void flushStream() = 0;

	// Begins a new class block.
	virtual void beginClass(const Class& type) = 0;

	// Ends the current class block.
	virtual void endClass() = 0;

	// Writes a field into the class block.
	virtual void writeField( const Field& field ) = 0;

	// Begins a new array block.
	virtual void beginArray(const Type& type) = 0;

	// Ends the current array block.
	virtual void endArray() = 0;

	// Primitive Primitives.
	virtual void writeBool( const Primitive&, bool ) = 0;
	virtual void writeInt( const Primitive&, int ) = 0;
	virtual void writeUInt( const Primitive&, uint ) = 0;
	virtual void writeFloat( const Primitive&, float ) = 0;
	virtual void writeDouble( const Primitive&, double ) = 0;
	virtual void writeString( const Primitive&, const std::string& ) = 0;
	virtual void writeVector3( const Primitive&, const Vector3& ) = 0;
	virtual void writeQuaternion( const Primitive&, const Quaternion& ) = 0;
	virtual void writeColor( const Primitive&, const Color& ) = 0;
	virtual void writeBitfield( const Primitive&, const std::bitset<32>& ) = 0;
	//virtual void writeHandle( const Handle& ) = 0;
};

struct ObjectData
{
	void* instance;
	Type* type;
};

//-----------------------------------//

/**
 * Serialization will write a type object to disk in a format that can
 * be readable later when loading. The serializator will use an abstract
 * class to format the data. This allows multiple serialization formats.
 */

class CORE_API ObjectSerializer
{
	DECLARE_UNCOPYABLE(ObjectSerializer)

public:

	ObjectSerializer( Serializer& serializer );

	// Loads an object from a stream.
	bool load( ObjectData& object );

	// Saves an object to a stream.
	void save( const ObjectData& object );

protected:

	// Serializes a type.
	void serializeType(ObjectData object);

	// Serializes a class.
	void serializeClass(ObjectData object);

	// Serializes the field of the object.
	void serializeField(ObjectData object, const Field& field);

	// Serializes a primitive field of the object.
	void serializePrimitive(const ObjectData& object);

	// Concrete serializer implementation.
	Serializer& serializer;
};

//-----------------------------------//

} // end namespace 