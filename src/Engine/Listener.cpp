/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "scene/Listener.h"
#include "scene/Transform.h"
#include "scene/Camera.h"
#include "scene/Entity.h"
#include "audio/Device.h"
#include "Engine.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Listener, Component)
	FIELD_PRIMITIVE_SETTER(float, volume, Volume)
END_CLASS()

//-----------------------------------//

Listener::Listener()
	: volume(0.75f)
	, audioContext(nullptr)
{
	AudioDevice* audioDevice = GetEngine()->getAudioDevice();
	audioContext = audioDevice->getMainContext();
}

//-----------------------------------//

Listener::~Listener()
{ }

//-----------------------------------//

void Listener::setVolume( float volume )
{
	this->volume = volume;
	audioContext->setVolume(volume);
}

//-----------------------------------//

void Listener::update( double delta )
{
	if(!audioContext)
		return;

	const EntityPtr& entity = getEntity();
	
	const CameraPtr& camera = entity->getComponent<Camera>();
	
	if(camera)
	{
		Vector3 lookAt = camera->getLookAtVector();
		lookAt.normalize();

		audioContext->setOrientation(lookAt);
	}

	const TransformPtr& transform = entity->getTransform();

	if(transform)
	{
		const Vector3& position = transform->getPosition();
		audioContext->setPosition(position);
	}
}

//-----------------------------------//

} // end namespace

#endif