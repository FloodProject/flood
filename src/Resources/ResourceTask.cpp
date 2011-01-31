/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceLoader.h"

#include "Core/File.h"
#include "Core/PhysfsStream.h"
#include "Utilities.h"

namespace vapor {

//-----------------------------------//

void ResourceTask::run()
{
	const std::string& path = resource->getPath();
	
	File file(path);
	PhysfsStream stream(file);

	ResourceManager* res = GetResourceManager();
	ResourceLoader* loader = res->findLoader( file.getExtension() );

	bool decoded = loader->decode(stream, resource);
		
	if( !decoded )
	{
		resource->setStatus( ResourceStatus::Error );
		Log::warn("Error decoding resource '%s'", path.c_str());
		return;
	}

	resource->setStatus( ResourceStatus::Loaded );

	if( options.sendLoadEvent )
	{
		ResourceEvent event;
		event.resource = resource;

		res->resourceTaskEvents.push(event);
	}

	res->numResourcesQueuedLoad.dec();
	THREAD( res->resourceFinishLoad.notify_one(); )

	Log::info("Loaded resource '%s'", path.c_str());
}

//-----------------------------------//

} // end namespace