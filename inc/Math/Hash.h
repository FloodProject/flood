/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_CORE_BEGIN

//-----------------------------------//

// Hashes some data using the Murmur2 hash.
// http://sites.google.com/site/murmurhash/

API_CORE uint32 HashMurmur2( uint32 seed, uint8* buf, uint32 size );

//-----------------------------------//

NAMESPACE_CORE_END