/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_AUDIO_OPENAL

#include "Engine/Audio/Buffer.h"
#include "Engine/Audio/Device.h"
#include "Engine/Audio/AL.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

AudioBuffer::AudioBuffer( AudioDevice* device )
    : device(device)
    , uploaded(false)
    , id(0)
{
    alGenBuffers(1, &id);

    // Check if the buffer was successfuly created.
    if(AudioCheckError()) 
    {
        LogWarn( "Error creating a sound buffer: %s", AudioGetError());
        return;
    }
}

//-----------------------------------//

AudioBuffer::~AudioBuffer()
{
    alDeleteBuffers(1, &id);

#if 0
    // Remove the audio buffer from the audio device.
    auto it = device->soundBuffers.Find(resource);

    if( it != device->soundBuffers.End())
        device->soundBuffers.Erase(resource);
#endif
}

//-----------------------------------//

void AudioBuffer::upload(const AudioBufferDetails& details)
{
    uploaded = false;
    
    AudioBufferData(id, details);

    uploaded = true;
    onBufferUploaded(this);
}

//-----------------------------------//

void AudioGetBufferDataDetails(AudioBufferDetails& details, Sound* sound)
{
    const Vector<byte>& soundData = sound->getBuffer();
    
    details.data = (uint8*) soundData.Buffer();
    details.size = soundData.Size();
    details.frequency = sound->getFrequency();
    details.format = AudioGetFormat(sound);
}

//-----------------------------------//

void AudioBufferData(ALuint buffer, const AudioBufferDetails& details)
{
    if( !details.data || details.size == 0 ) return;
    
    // Uploads sound data to the buffer.
    alBufferData( buffer, details.format,
        details.data, details.size, details.frequency );
    
    if(AudioCheckError())
    {
        LogWarn("Error uploading sound to buffer: %s", AudioGetError());
        return;
    }
}

//-----------------------------------//

void AudioBufferSound(AudioBuffer* buffer, Sound* sound)
{
    AudioBufferDetails details;
    AudioGetBufferDataDetails(details, sound);

    assert( buffer != nullptr );
    buffer->upload(details);
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif