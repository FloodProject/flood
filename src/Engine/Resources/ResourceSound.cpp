/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Resources/Sound.h"
#include "Core/Stream.h"

NAMESPACE_ENGINE_BEGIN

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
	, streamed(false)
{

}

//-----------------------------------//

NAMESPACE_ENGINE_END
