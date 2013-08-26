/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"

#include "Core/Serialization.h"
#include "Core/Reflection.h"
#include "Core/Stream.h"

#ifdef ENABLE_SERIALIZATION

typedef struct json_t json_t;

NAMESPACE_CORE_BEGIN

//-----------------------------------//

class Field;

class API_CORE SerializerBinary : public Serializer
{
public:

	/**
	 * Creates a new binary serializer.
	 * @param allocator allocator used for this serializer
	 * @param handleContextMap map used to map classes to their serializaton/deserialization functions
	 */
	SerializerBinary(Allocator* allocator, ReflectionHandleContextMap* handleContextMap);

	/**
	 * Loads an object from stream.
	 */
	virtual Object* load() override;

	/**
	 * Saves an object to stream.
	 * @param obj object to save
	 */
	virtual bool save(const Object* obj) override;

	MemoryStream* ms; //<! memory stream used for serialization/deserialization
};

//-----------------------------------//

class API_CORE SerializerJSON : public Serializer
{
public:

	/**
	 * Creates a new JSON serializer.
	 * @param allocator allocator used for this serializer
	 * @param handleContextMap map used to map classes to their serializaton/deserialization functions
	 */
	SerializerJSON(Allocator* alloc, ReflectionHandleContextMap* handleContextMap);

	/**
	 * Loads an object from stream.
	 */
	virtual Object* load() override;

	/**
	 * Saves an object to stream.
	 * @param obj object to save
	 */
	virtual bool save(const Object* obj) override;

	json_t* rootValue; //!< Root JSON value.

	std::vector<json_t*> values; //!< Stack of JSON values.
};

ValueContext ConvertValueToPrimitive( PrimitiveTypeKind kind, json_t* value );

//-----------------------------------//

// Deserializes a field from a stream.
API_CORE void DeserializeFields( ReflectionContext* context );

template<typename T>
static void PointerSetObject( const Field* field, void* address, T* object )
{
	if( FieldIsRawPointer(field) )
	{
		T** raw = (T**) address;
		*raw = object;
	}
	else if( FieldIsRefPointer(field) )
	{
		T* ref = (T*) object;
		RefPtr<T>* ref_obj = (RefPtr<T>*) address;
		ref_obj->reset(ref);
	}
	else if( FieldIsHandle(field) )
	{
		assert(0 && "Not implemented");
	}
	else
	{
		assert(0 && "Not implemented");
		//memcpy(element, object, size);
	}
}

//-----------------------------------//

/**
 * Wire format is inspired on Google Protocol Buffers.
 * http://code.google.com/apis/protocolbuffers/docs/encoding.html
 */

#define StreamIndex(s) (s->buffer+s->position)

#ifdef BUILD_DEBUG
void StreamAdvanceIndex(MemoryStream* ms, uint64 n);
#else
#define StreamAdvanceIndex(s, n) (s->position += n)
#endif

void EncodeVariableIntegerBuffer(uint8* buf, uint64& advance, uint64 val);
void API_CORE EncodeVariableInteger(MemoryStream* ms, uint64 val);
bool DecodeVariableIntegerBuffer(uint8* buf, uint64& advance, uint64& val);
bool DecodeVariableInteger(MemoryStream* ms, uint64& val);
uint32 EncodeZigZag32(int32 n);
int32  DecodeZigZag32(uint32 n);
uint64 EncodeZigZag64(int64 n);
int64  DecodeZigZag64(uint64 n);
void EncodeFixed32(MemoryStream* ms, uint32 val);
uint32 DecodeFixed32(MemoryStream* ms);
void EncodeFloat(MemoryStream* ms, float val);
float DecodeFloat(MemoryStream* ms);
void EncodeString(MemoryStream* ms, const String& s);
bool DecodeString(MemoryStream* ms, String& s);

// Array helpers.
void* ReflectionArrayResize( ReflectionContext*, void* address, uint32 size );
uint16 ReflectionArrayGetElementSize(const Field* field);

//-----------------------------------//

NAMESPACE_CORE_END

#endif