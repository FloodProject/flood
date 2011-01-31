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

class Thread
{
};

//-----------------------------------//

} // end namespace

#ifdef VAPOR_THREADING
	#define THREAD( code ) code
#else
	#define THREAD( code )
#endif