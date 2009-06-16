/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_IMAGE_PICOPNG

#include "vapor/resources/OGG_Loader.h"

using vapor::vfs::File;

#include <vorbis/vorbisfile.h>

#ifdef VAPOR_PLATFORM_WINDOWS
	// disable Visual C++ fopen deprecation warning
	#pragma warning(disable : 4996)
#endif

//-----------------------------------//

namespace vapor {
	namespace resources {

//-----------------------------------//

OGG_Loader::OGG_Loader()
{
	extensions.push_back("ogg");
}

//-----------------------------------//

Sound* OGG_Loader::decode(File& file)
{
	int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
	int bitStream;
	long bytes;
	const unsigned long BUFFER_SIZE( 32768 );
	char array[BUFFER_SIZE];    // Local fixed size array

	// Open for binary reading
	FILE* f( fopen(file.getPath().c_str(), "rb") );

	OggVorbis_File oggFile;

	ov_open(f, &oggFile, NULL, 0);

	// Get some information about the OGG file
	vorbis_info* pInfo( ov_info(&oggFile, -1) );

	SoundFormat::Enum format;

	// Check the number of channels... always use 16-bit samples
	if (pInfo->channels == 1)
		format = SoundFormat::MONO16;
	else
		format = SoundFormat::STEREO16;

	int frequency( pInfo->rate );
	std::vector < char > buffer;

	do 
	{
		// Read up to a buffer's worth of decoded sound data
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
		
		// Append to end of buffer
		buffer.insert(buffer.end(), array, array + bytes);
	} while (0 < bytes);

	// This saves some memory by freeing the unused capacity part of the vector
	std::vector< char >( buffer ).swap( buffer );

	ov_clear(&oggFile);

	// The frequency of the sampling rate
	return new Sound( format, frequency, buffer );
}

//-----------------------------------//

} } // end namespaces

#endif