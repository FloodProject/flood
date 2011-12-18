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

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(OGG_Loader, ResourceLoader)
REFLECT_CLASS_END()

//-----------------------------------//

static size_t OggRead(void* ptr, size_t size, size_t nmemb, void* datasource)
{
	Stream* stream = (Stream*) datasource;
	return StreamReadBuffer(stream, ptr, size*nmemb);
}

//-----------------------------------//

static int OggSeek(void* datasource, ogg_int64_t offset, int whence)
{
	Stream* stream = (Stream*) datasource;

	StreamSeekMode::Enum mode;

	switch(whence)
	{
	case SEEK_SET:
		mode = StreamSeekMode::Absolute;
		break;
	case SEEK_CUR:
		mode = StreamSeekMode::Relative;
		break;
	case SEEK_END:
		mode = StreamSeekMode::RelativeEnd;
		break;
	}

	StreamSetPosition(stream, offset, mode);

	return 0;
}

//-----------------------------------//

static int OggClose(void* /*datasource*/)
{
	return 0;
}

//-----------------------------------//

static long OggTell(void* datasource)
{
	Stream* stream = (Stream*) datasource;
	return StreamGetPosition(stream);
}

//-----------------------------------//

struct OggStream : public SoundStream
{
	~OggStream();

	// Streams a chunk of the resource.
	int decode(uint8* buffer, size_t size) OVERRIDE;

	// Resets the stream to the beginning.
	void reset() OVERRIDE;

	OggVorbis_File* ogg;
	size_t begin;
};

//-----------------------------------//

int OggStream::decode(uint8* buffer, size_t bufSize)
{
	// Decode the sound into a buffer now.
	int endianness = SystemIsLittleEndian() ? 0 : 1;
	
	size_t size = 0;
	int bitStream;

	// Read up to a buffer's worth of decoded sound data.
	while(size < bufSize) 
	{
		size_t read = ov_read(ogg, (char*) buffer + size, bufSize- size, 
			endianness,
			2, // 1 for 8-bit samples, or 2 for 16-bit samples
			1, // 0 for unsigned, 1 for signed
			&bitStream);

		if( read <= 0 ) break;

		size += read;
	}

	return size;
}

//-----------------------------------//

OggStream::~OggStream()
{
	ov_clear(ogg);
	Deallocate(ogg);
};

//-----------------------------------//

void OggStream::reset()
{
	if(!ogg) return;
	ov_pcm_seek(ogg, 0);
}

//-----------------------------------//

OGG_Loader::OGG_Loader()
{
	extensions.push_back("ogg");

	callbacks.read_func = OggRead;
	callbacks.seek_func = OggSeek;
	callbacks.close_func = nullptr;
	callbacks.tell_func = OggTell;
}

//-----------------------------------//

Resource* OGG_Loader::prepare(ResourceLoadOptions& options)
{
	ResourceLoadOption& option = options.option;
	bool streamed = option.key == SOUND_LOAD_STREAM && option.value;
	
	Sound* sound = AllocateThis(Sound);
	if( !streamed) return sound;
	
	sound->setStreamed(true);
	options.keepStreamOpen = true;
	options.resource = sound;

	OggVorbis_File* oggFile = AllocateThis(OggVorbis_File);

	if( !initOgg(oggFile, options) )
	{
		Deallocate(oggFile);
		Deallocate(sound);
		return nullptr;
	}

	OggStream* oggStream = AllocateThis(OggStream);
	oggStream->loader = this;
	oggStream->stream = options.stream;
	oggStream->ogg = oggFile;
	oggStream->begin = oggStream->ogg->offset;

	sound->stream = oggStream;

	return sound;
}

//-----------------------------------//

bool OGG_Loader::initOgg( OggVorbis_File*& oggFile, ResourceLoadOptions& options )
{
	int res = ov_open_callbacks((void*)options.stream, oggFile, nullptr, 0, callbacks);

	// Get some information about the OGG file.
	vorbis_info* pInfo = ov_info(oggFile, -1);
	
	// If the OGG could not be opened, return a null resource.
	if(!pInfo) return false;

	Sound* sound = static_cast<Sound*>( options.resource );
	sound->setSize(16);
	sound->setChannels( pInfo->channels );
	sound->setFrequency( pInfo->rate );

	return true;
}

//-----------------------------------//

bool OGG_Loader::decode(ResourceLoadOptions& options)
{
	Sound* sound = static_cast<Sound*>( options.resource );
	
	if( sound->getStreamed() )
		return true;

	OggVorbis_File oggFile;
	OggVorbis_File* p_oggFile = &oggFile;
	
	// Initialize the sound. 
	if( !initOgg(p_oggFile, options) )
		return false;

	// Decode the sound into a buffer now.
	std::vector<byte>& buffer = sound->dataBuffer;
	decodeOgg( &oggFile, buffer );

	ov_clear(&oggFile);

	return true;
}

//-----------------------------------//

void OGG_Loader::decodeOgg( OggVorbis_File* oggFile, std::vector<byte>& buffer )
{
	// Decode the sound into a buffer now.
	const size_t BUFFER_SIZE = 32768;
	int endianness = SystemIsLittleEndian() ? 0 : 1;
	
	int bytes, bitStream;

	do
	{
		byte array[BUFFER_SIZE];

		// Read up to a buffer's worth of decoded sound data.
		bytes = ov_read(oggFile, (char*) array, BUFFER_SIZE, 
			endianness, // 0 for Little-Endian, 1 for Big-Endian
			2, // 1 for 8-bit samples, or 2 for 16-bit samples
			1, // 0 for unsigned, 1 for signed
			&bitStream);
		
		// Append to end of buffer.
		buffer.insert(buffer.end(), array, array + bytes);

	} while (bytes > 0);
}

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif