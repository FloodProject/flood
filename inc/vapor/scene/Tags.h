/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

/**
 * We define some default tags that we need and/or find useful.
 * Note that position 24 and beyond are reserved for internal engine use.
 */

namespace Tags
{
	enum Bitfield
	{
		NonPickable				= 1 << 24,
		NonTransformable		= 1 << 25,
		NonCollidable			= 1 << 26,
		NonCulled				= 1 << 27,
		UpdateTransformsOnly	= 1 << 28,
	};
}

//-----------------------------------//

} // end namespace