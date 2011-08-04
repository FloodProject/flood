/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/Sound.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Sound, Resource)
	FIELD_PRIMITIVE(4, int32, frequency) FIELD_READONLY(frequency)
	FIELD_PRIMITIVE(5, int32, channels) FIELD_READONLY(channels)
REFLECT_CLASS_END()

//-----------------------------------//

Sound::Sound()
	: frequency(-1)
	, channels(0)
	, size(0)
{ }

//-----------------------------------//

NAMESPACE_RESOURCES_END