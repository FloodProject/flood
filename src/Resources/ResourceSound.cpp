/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/Sound.h"

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(Sound, Resource)
	FIELD_PRIMITIVE(int32, frequency) FIELD_READONLY(frequency)
	FIELD_PRIMITIVE(int32, channels) FIELD_READONLY(channels)
REFLECT_CLASS_END()

//-----------------------------------//

Sound::Sound()
	: frequency(-1)
	, channels(0)
	, size(0)
{ }

//-----------------------------------//

} // end namespace