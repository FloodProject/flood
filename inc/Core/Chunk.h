/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Stream.h"

NAMESPACE_BEGIN

//-----------------------------------//

/**
 * Each chunk is identified by a piece of metadata that consists of an id
 * (see: FourCC) for easy identification of each chunks purpose.
 */

struct API_CORE ChunkMetadata
{
	uint8 critical : 1;
	uint8 version  : 2;
	uint8 type     : 5;
};

struct API_CORE Chunk
{
	ChunkMetadata metadata;
	uint32 id;
	uint32 size;
	byte* data;

	// Makes an identifier.
	static uint32 makeId(const String& id);
	static uint32 makeHashId(const String& id);
};

//-----------------------------------//

NAMESPACE_END