/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/resources/Sound.h"

namespace vapor {
	namespace resources {

//-----------------------------------//

namespace SoundFormat
{
	string getString(Enum e) 
	{
		switch(e) 
		{
		case Mono8:
		  return "MONO8";
		case Mono16:
		  return "MONO16";
		case Stereo8:
		  return "STEREO8";
		case Stereo16:
		  return "STEREO16";
		default:
			return nullptr;
		}
	}
}

//-----------------------------------//

Sound::Sound(const SoundFormat::Enum format_, const int frequency_, std::vector< char > const& data)
	: frequency( frequency_ ), format(format_), dataBuffer( data )
{
}

//-----------------------------------//

Sound::~Sound()
{
}

//-----------------------------------//

} } // end namespaces