/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Graphics/API.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * "Static" means the data in VBO will not be changed (specified once and used
 * many times), "dynamic" means the data will be changed frequently (specified
 * and used repeatedly), and "stream" means the data will be changed every 
 * frame (specified once and used once). "Draw" means the data will be sent to
 * GPU in order to draw (application to GL), "read" means the data will be read
 * by the client's application (GL to application), and "copy" means the data
 * will be used both drawing and reading (GL to GL).
 */

enum struct BufferUsage : uint8
{
	Static,  // Content rarely changes
	Stream,  // Content sometimes changes
	Dynamic  // Content always changes
};

//-----------------------------------//

/**
 * Use these enums to represent the lifetime and usage patterns of a 
 * buffer. These help the engine make better decisions about where to
 * store the buffers, which leads to better rendering performance.
 */

enum struct BufferAccess : uint8
{
	Read,     // Read content only
	Write,    // Write content only
	ReadWrite // Read and write content
};

//-----------------------------------//

class GeometryBuffer;

/**
 * Represents a buffer of data stored in memory. They mainly store
 * rendering data given to the render API. The buffers may be stored 
 * in different kinds of memory, like system RAM or video RAM,
 * depending on the usage and type flags that they are  created with. 
 */

typedef uint32 BufferId;

struct API_GRAPHICS Buffer : public ReferenceCounted
{
	Buffer();
	Buffer(BufferUsage usage, BufferAccess access);

	// Note: The destructor isn't virtual because this base class
	// isn't meant to be used polymorphically. We don't want the overhead.

	~Buffer();

	// Gets/sets the buffer usage type.
	ACCESSOR(BufferUsage, BufferUsage, usage)

	// Gets/sets the buffer access type.
	ACCESSOR(BufferAccess, BufferAccess, access)

	// Gets/sets the geometry buffer.
	ACCESSOR(GeometryBuffer, const GeometryBuffer*, gb)

	BufferId id;
	BufferUsage usage;
	BufferAccess access;

	// Keeps the geometry buffer associated with this buffer.
	const GeometryBuffer* gb;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Buffer );

//-----------------------------------//

NAMESPACE_GRAPHICS_END
