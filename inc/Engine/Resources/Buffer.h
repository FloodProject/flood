/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/References.h"

NAMESPACE_RESOURCES_BEGIN

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
	enum Enum : uint8
	{
		Static,  // Content rarely changes
		Stream,  // Content sometimes changes
		Dynamic  // Content always changes
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
	enum Enum : uint8
	{
		Read,     // Read content only
		Write,    // Write content only
		ReadWrite // Read and write content
	};
};

//-----------------------------------//

NAMESPACE_RESOURCES_END
