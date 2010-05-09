/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_AUDIO_OGG

#include "vapor/resources/OGG_Loader.h"

using vapor::vfs::File;

namespace vapor { namespace resources {

//-----------------------------------//

size_t read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	const File& file = *reinterpret_cast< File* >( datasource );
	return file.read( ptr, size*nmemb );
}

//-----------------------------------//

int seek_func(void* /*datasource*/, ogg_int64_t /*offset*/, int /*whence*/)
{
	//const File& file = *reinterpret_cast< File* >( datasource );
	////return file.seek( ... );
	return 0;
}

//-----------------------------------//

int close_func(void* /*datasource*/)
{
	//const File& file = *reinterpret_cast< File* >( datasource );
	////return file.close( ... );
	return 0;
}

//-----------------------------------//

long tell_func(void *datasource)
{
	const File& file = *reinterpret_cast< File* >( datasource );
	return file.tell();
}

//-----------------------------------//

OGG_Loader::OGG_Loader()
{
	extensions.push_back("ogg");

	callbacks.read_func = read_func;
	callbacks.seek_func = nullptr /*seek_func*/;
	callbacks.close_func = nullptr /*close_func*/;
	callbacks.tell_func = tell_func;
}

//-----------------------------------//

bool OGG_Loader::decode(const File& file, Resource* res)
{
	OggVorbis_File oggFile;
	ov_open_callbacks((void*) &file, &oggFile, nullptr, 0, callbacks);

	// Get some information about the OGG file.
	vorbis_info* pInfo( ov_info(&oggFile, -1) );
	
	// If the OGG could not be opened, return a null resource.
	if(!pInfo)
		return false;

	// Decode the sound into a buffer now.
	std::vector<byte> buffer;
	decodeOgg( &oggFile, buffer );
	std::vector<byte>( buffer ).swap( buffer );

	ov_clear(&oggFile);

	Sound* sound = static_cast<Sound*>( res );
	sound->setChannels( pInfo->channels );
	sound->setFrequency( pInfo->rate );
	sound->setBuffer( buffer );

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
			System::isLittleEndian() ? 0 : 1, // 0 for Little-Endian, 1 for Big-Endian
			2, // 1 for 8-bit samples, or 2 or 16-bit samples
			1, // 0 for unsigned, 1 for signed
			&bitStream);
		
		// Append to end of buffer.
		buffer.insert(buffer.end(), array, array + bytes);

	} while (bytes > 0);
}

//-----------------------------------//

} } // end namespaces

#endif