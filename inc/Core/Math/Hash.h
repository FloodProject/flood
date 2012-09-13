/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
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