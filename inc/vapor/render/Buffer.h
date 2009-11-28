/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/render/GL.h"

namespace vapor { namespace render {

//"Static" means the data in VBO will not be changed (specified once and used
//many times), "dynamic" means the data will be changed frequently (specified
//and used repeatedly), and "stream" means the data will be changed every 
//frame (specified once and used once). "Draw" means the data will be sent to
//GPU in order to draw (application to GL), "read" means the data will be read
//by the client's application (GL to application), and "copy" means the data
//will be used both drawing and reading (GL to GL). 

//-----------------------------------//

namespace BufferUsage
{
	enum Enum
	{
		Static,		// Content rarely changes
		Stream,		// Content sometimes changes
		Dynamic		// Content always changes
	};
}

//-----------------------------------//

/**
 * Use these enums to represent the lifetime and usage patterns of a 
 * buffer. These help the engine make better decisions about where to
 * store the buffers, which leads to better rendering performance.
 */

namespace BufferAccess
{
	enum Enum
	{
		Read,		// Read content only
		Write,		// Write content only
		ReadWrite	// Read and write content
	};
}

//-----------------------------------//

/**
 * Represents a buffer of data stored in memory. They mainly store
 * rendering data given to the render API. The buffers may be stored 
 * in different kinds of memory, like system RAM or video RAM,
 * depending on the usage and type flags that they are  created with. 
 */

class VAPOR_API Buffer
{
public:

	Buffer();
	virtual ~Buffer();

protected:

	// Converts the buffer enums to the equivalent GL ones.
	GLenum getGLBufferType( BufferUsage::Enum bU, BufferAccess::Enum bA );

    uint id;
};

//-----------------------------------//

} } // end namespaces
