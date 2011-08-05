/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_SERIALIZATION

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct Field;

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

#define StreamIndex(s) ((&s->data[0])+s->position)
#define StreamAdvanceIndex(s, n) (s->position += n)

static void EncodeVariableInteger(MemoryStream* ms, uint64 val)
{
	uint8 n = 0;
	uint8* buf = StreamIndex(ms);

	do
	{
		uint8 byte = val & 0x7f;
		val >>= 7;
		if(val) byte |= 0x80;
		*buf++ = byte;
		n++;
	}
	while(val);

	StreamAdvanceIndex(ms, n);
}

static bool DecodeVariableInteger(MemoryStream* ms, uint64& val)
{
	uint8* buf = StreamIndex(ms);
	uint8* p = buf;

	uint32 low, high = 0;
	uint32 b;
	b = *(p++); low   = (b & 0x7f)      ; if(!(b & 0x80)) goto done;
	b = *(p++); low  |= (b & 0x7f) <<  7; if(!(b & 0x80)) goto done;
	b = *(p++); low  |= (b & 0x7f) << 14; if(!(b & 0x80)) goto done;
	b = *(p++); low  |= (b & 0x7f) << 21; if(!(b & 0x80)) goto done;
	b = *(p++); low  |= (b & 0x7f) << 28;
				high  = (b & 0x7f) >>  4; if(!(b & 0x80)) goto done;
	b = *(p++); high |= (b & 0x7f) <<  3; if(!(b & 0x80)) goto done;
	b = *(p++); high |= (b & 0x7f) << 10; if(!(b & 0x80)) goto done;
	b = *(p++); high |= (b & 0x7f) << 17; if(!(b & 0x80)) goto done;
	b = *(p++); high |= (b & 0x7f) << 24; if(!(b & 0x80)) goto done;
	b = *(p++); high |= (b & 0x7f) << 31; if(!(b & 0x80)) goto done;
	return false;

done:

	StreamAdvanceIndex(ms, p-buf);
	val = ((uint64) high << 32) | low;
	return true;
}


//-----------------------------------//

/**
 * For signed numbers, we use an encoding called zig-zag encoding
 * that maps signed numbers to unsigned numbers so they can be
 * efficiently encoded using the normal variable-int encoder.
 */

static uint32 EncodeZigZag32(sint32 n)
{
	return (n << 1) ^ (n >> 31);
}

static sint32 DecodeZigZag32(uint32 n)
{
	return (n >> 1) ^ -(sint32)(n & 1);
}

static uint64 EncodeZigZag64(sint64 n)
{
	return (n << 1) ^ (n >> 63);
}

static sint64 DecodeZigZag64(uint64 n)
{
	return (n >> 1) ^ -(sint64)(n & 1);
}

//-----------------------------------//

static void EncodeFixed32(MemoryStream* ms, uint32 val)
{
	uint8* buf = StreamIndex(ms);
	buf[0] = val & 0xff;
	buf[1] = (val >> 8) & 0xff;
	buf[2] = (val >> 16) & 0xff;
	buf[3] = (val >> 24);
	StreamAdvanceIndex(ms, sizeof(uint32));
}

static uint32 DecodeFixed32(MemoryStream* ms)
{
	uint8* buf = StreamIndex(ms);
	uint32* val = (uint32*) buf;
	StreamAdvanceIndex(ms, sizeof(uint32));
	return *val;
}

//-----------------------------------//

static void EncodeString(MemoryStream* ms, const String& s)
{
	EncodeVariableInteger(ms, s.size());
	StreamWriteString(ms, s);
}

static bool DecodeString(MemoryStream* ms, String& s)
{
	uint64 size;
	
	if( !DecodeVariableInteger(ms, size) )
		return false;

	s.resize((size_t)size);
	memcpy(&s[0], StreamIndex(ms), (size_t)size);

	StreamAdvanceIndex(ms, size);

	return true;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif