/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "resources/Sound.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT_ABSTRACT(Sound, Resource)
END_CLASS()

//-----------------------------------//

Sound::Sound()
	: frequency(-1)
	, channels(0)
	, size(0)
{ }

//-----------------------------------//

} // end namespace