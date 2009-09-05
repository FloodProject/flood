/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/CompileOptions.h"

#ifdef VAPOR_AUDIO_OGG

#include "vapor/resources/OGG_Loader.h"

using vapor::vfs::File;

#include <vorbis/vorbisfile.h>

#ifdef VAPOR_COMPILER_MSVC
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
	// TODO: Use the NativeFile/File class for I/O
	FILE* f( fopen(file.getPath().c_str(), "rb") );
	
	//check if the file was opened successfully
	if(!f) return nullptr;
	
	OggVorbis_File oggFile;
	
	ov_open(f, &oggFile, nullptr, 0);

	// Get some information about the OGG file
	vorbis_info* pInfo( ov_info(&oggFile, -1) );
	
	if(!pInfo) return nullptr;

	SoundFormat::Enum format;

	// Check the number of channels... always use 16-bit samples
	if (pInfo->channels == 1)
		format = SoundFormat::Mono8; // TODO: Are 8-bit samples valid here?
	else
		format = SoundFormat::Stereo16;

	int frequency( pInfo->rate );
	std::vector<byte> buffer;

	do 
	{
		// Read up to a buffer's worth of decoded sound data
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
		
		// Append to end of buffer
		buffer.insert(buffer.end(), array, array + bytes);

	} while (bytes > 0);

	// This saves some memory by freeing the unused capacity part of the vector
	std::vector<byte>( buffer ).swap( buffer );

	ov_clear(&oggFile);

	Sound* sound = new Sound( format, frequency, buffer );
	sound->setPath( file.getPath() );

	return sound;
}

//-----------------------------------//

} } // end namespaces

#endif