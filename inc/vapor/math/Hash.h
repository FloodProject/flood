/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_BEGIN

//-----------------------------------//

namespace Hash
{
	// Hashes some data using the Murmur2 hash.
	// http://sites.google.com/site/murmurhash/
	uint Murmur2( const std::vector<byte>& data, uint seed );
}

//-----------------------------------//

NAMESPACE_END