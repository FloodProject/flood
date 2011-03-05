/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "ReferenceCount.h"

namespace vapor {

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

struct BufferUsage
{
	enum Enum
	{
		Static,		// Content rarely changes
		Stream,		// Content sometimes changes
		Dynamic		// Content always changes
	};
};

//-----------------------------------//

/**
 * Use these enums to represent the lifetime and usage patterns of a 
 * buffer. These help the engine make better decisions about where to
 * store the buffers, which leads to better rendering performance.
 */

struct BufferAccess
{
	enum Enum
	{
		Read,		// Read content only
		Write,		// Write content only
		ReadWrite	// Read and write content
	};
};

//-----------------------------------//

/**
 * Represents a buffer of data stored in memory. They mainly store
 * rendering data given to the render API. The buffers may be stored 
 * in different kinds of memory, like system RAM or video RAM,
 * depending on the usage and type flags that they are  created with. 
 */

class VAPOR_API Buffer : public ReferenceCounted
{
public:

	Buffer();
	Buffer(BufferUsage::Enum usage, BufferAccess::Enum access);

	// Note: The destructor isn't virtual because this base class
	// isn't meant to be used polymorphically. We don't want the overhead.

	~Buffer();

	// Gets/sets the buffer usage type.
	ACESSOR(BufferUsage, BufferUsage::Enum, usage)

	// Gets/sets the buffer access type.
	ACESSOR(BufferAccess, BufferAccess::Enum, access)

protected:

	// Converts the buffer enums to the equivalent GL ones.
	int getGLBufferType();

	uint id;
	
	BufferUsage::Enum usage;
	BufferAccess::Enum access;
};

//-----------------------------------//

} // end namespace
