/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

/**
 * Use these for different kinds of access to the streams.
 */

namespace StreamMode
{
	enum Enum { Read, Write, Append };
}

/**
 * Controls where the seeking will occur from.
 */

namespace StreamSeekMode
{
	enum Enum { Absolute, Relative, RelativeEnd };
}

//-----------------------------------//

/**
 * A stream allows uniform access to data backed by different storage
 * mediums like memory, files, archives, or even in remove servers.
 */

struct Stream;

typedef bool  (*StreamOpenFunction)(Stream*);
typedef bool  (*StreamCloseFunction)(Stream*);
typedef int64 (*StreamReadFunction)(Stream*, void*, int64);
typedef int64 (*StreamWriteFunction)(Stream*, void*, int64);
typedef int64 (*StreamTellFunction)(Stream*);
typedef bool  (*StreamSeekFunction)(Stream*, int64, int8);
typedef int64 (*StreamSizeFunction)(Stream*);

struct StreamFuncs
{
	StreamOpenFunction  open;
	StreamCloseFunction close;
	StreamReadFunction  read;
	StreamWriteFunction write;
	StreamTellFunction  tell;
	StreamSeekFunction  seek;
	StreamSizeFunction  size;
};

struct Stream
{
	String path;
	StreamMode::Enum mode;
	StreamFuncs* fn;
};

/**
 * Streams can be created from URI (Univeral Resource Identifiers).
 * Each stream protocol supported has a unique scheme that will be
 * used to identify it when opening the stream. Examples, http:,
 * or zip: would be valid schemes for an HTTP or ZIP stream.
 */

API_CORE Stream*  StreamCreateFromURI(MemoryAllocator*, const Path&, StreamMode::Enum);
API_CORE Stream*  StreamCreateFromFile(MemoryAllocator*, const Path&, StreamMode::Enum);
API_CORE Stream*  StreamCreateFromPhysfs(MemoryAllocator*, const Path&, StreamMode::Enum);

API_CORE void   StreamDestroy(Stream*, MemoryAllocator*);
API_CORE bool   StreamClose(Stream*);

API_CORE int64  StreamRead(Stream*, std::vector<uint8>& data);
API_CORE int64  StreamReadString(Stream*, String&);
API_CORE int64  StreamReadLines(Stream*, std::vector<String>&);

API_CORE int64  StreamWrite(Stream*, const std::vector<uint8>&);
API_CORE int64  StreamWriteString(Stream*, const String&);

API_CORE int64  StreamGetPosition(Stream*);
API_CORE int64  StreamSetPosition(Stream*, int64, StreamSeekMode::Enum);

API_CORE int64  StreamGetSize(Stream*);

//-----------------------------------//

NAMESPACE_EXTERN_END