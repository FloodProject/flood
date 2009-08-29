/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

namespace vapor {
	namespace render {

/**
 * Use these enums to represent the lifetime and usage patterns of a 
 * buffer. These help the engine make better decisions about where to
 * store the buffers, which leads to better rendering performance.
 */

namespace BufferUsage
{
	enum Enum
	{
		Write,		// Write content only
		Read,		// Read content only
		ReadWrite	// Read and write content
	};
}

namespace BufferType
{
	enum Enum
	{
		Static,		// Content rarely changes
		Stream,		// Content sometimes changes
		Dynamic		// Content always changes
	};
}

/**
 * Represents a buffer of data stored in memory. They mainly store
 * rendering data given to the render API for. The buffers can be 
 * stored in different kinds of memory, like system RAM or video RAM,
 * depending on the usage and type flags that they are  created with. 
 */

class Buffer
{
public:

	Buffer();
	virtual ~Buffer();
};

} } // end namespaces
