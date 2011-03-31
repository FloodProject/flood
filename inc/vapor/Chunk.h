/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Stream.h"

namespace vapor {

//-----------------------------------//

/**
 * Each chunk is identified by a piece of metadata that consists of an id
 * (see: FourCC) for easy identification of each chunks purpose.
 */

struct API_CORE ChunkMetadata
{
	byte critical : 1;
	byte version  : 2;
	byte type     : 5;
};

struct API_CORE Chunk
{
	ChunkMetadata metadata;
	uint id;
	uint size;
	byte* data;

	// Makes an identifier.
	static uint makeId(const String& id);
	static uint makeHashId(const String& id);
};

//-----------------------------------//

} // end namespace