/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Resources/Buffer.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Represents a buffer of data stored in memory. They mainly store
 * rendering data given to the render API. The buffers may be stored 
 * in different kinds of memory, like system RAM or video RAM,
 * depending on the usage and type flags that they are  created with. 
 */

typedef uint32 BufferId;

class API_RENDER Buffer : public ReferenceCounted
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

	BufferId id;
	BufferUsage::Enum usage;
	BufferAccess::Enum access;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Buffer );

//-----------------------------------//

NAMESPACE_GRAPHICS_END
