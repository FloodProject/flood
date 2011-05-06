/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef ENABLE_AUDIO_OGG

#include "Resources/OGG_Loader.h"
#include "Core/Utilities.h"

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(OGG_Loader, ResourceLoader)
REFLECT_CLASS_END()

//-----------------------------------//

static size_t read_func(void* ptr, size_t size, size_t nmemb, void* datasource)
{
	Stream* stream = (Stream*) datasource;
	return StreamReadBuffer(stream, ptr, size*nmemb);
}

//-----------------------------------//

static int seek_func(void* /*datasource*/, ogg_int64_t /*offset*/, int /*whence*/)
{
	return 0;
}

//-----------------------------------//

static int close_func(void* /*datasource*/)
{
	return 0;
}

//-----------------------------------//

static long tell_func(void* datasource)
{
	Stream* stream = (Stream*) datasource;
	return StreamGetPosition(stream);
}

//-----------------------------------//

OGG_Loader::OGG_Loader()
{
	extensions.push_back("ogg");

	callbacks.read_func = read_func;
	callbacks.seek_func = nullptr;
	callbacks.close_func = nullptr;
	callbacks.tell_func = tell_func;
}

//-----------------------------------//

bool OGG_Loader::decode(const Stream& stream, Resource* res)
{
	OggVorbis_File oggFile;
	ov_open_callbacks((void*) &stream, &oggFile, nullptr, 0, callbacks);

	// Get some information about the OGG file.
	// To retrieve the vorbis_info struct for the current bitstream,
	// the second parameter should be set to -1.
	vorbis_info* pInfo = ov_info(&oggFile, -1);
	
	// If the OGG could not be opened, return a null resource.
	if(!pInfo) return false;

	// Decode the sound into a buffer now.
	std::vector<byte> buffer;
	decodeOgg( &oggFile, buffer );
	std::vector<byte>( buffer ).swap( buffer );

	Sound* sound = static_cast<Sound*>( res );
	sound->setSize(16);
	sound->setChannels( pInfo->channels );
	sound->setFrequency( pInfo->rate );
	sound->setBuffer( buffer );

	ov_clear(&oggFile);

	return true;
}

//-----------------------------------//

void OGG_Loader::decodeOgg( OggVorbis_File* oggFile, std::vector<byte>& buffer )
{
	// Decode the sound into a buffer now.
	const uint BUFFER_SIZE( 32768 );
	int bytes, bitStream;

	do 
	{
		byte array[BUFFER_SIZE];

		// Read up to a buffer's worth of decoded sound data.
		bytes = ov_read(oggFile, (char*) array, BUFFER_SIZE, 
			SystemIsLittleEndian() ? 0 : 1, // 0 for Little-Endian, 1 for Big-Endian
			2, // 1 for 8-bit samples, or 2 or 16-bit samples
			1, // 0 for unsigned, 1 for signed
			&bitStream);
		
		// Append to end of buffer.
		buffer.insert(buffer.end(), array, array + bytes);

	} while (bytes > 0);
}

//-----------------------------------//

} // end namespace

#endif