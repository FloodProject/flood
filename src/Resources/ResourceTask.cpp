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
#include "Core/Memory.h"

#include "Log.h"
#include "Utilities.h"


namespace vapor {

//-----------------------------------//

void ResourceTaskRun(Task* task)
{
	ResourceLoadOptions* options = (ResourceLoadOptions*) task->Userdata;
	Resource* resource = options->resource;
	const Path& path = resource->getPath();
	
	File file(path, StreamMode::Read);
	PhysfsStream stream(file);

	ResourceManager* res = GetResourceManager();
	ResourceLoader* loader = res->findLoader( PathGetFileExtension(path) );

	bool decoded = loader->decode(stream, resource);
		
	if( !decoded )
	{
		resource->setStatus( ResourceStatus::Error );
		LogWarn("Error decoding resource '%s'", path.c_str());
		return;
	}

	resource->setStatus( ResourceStatus::Loaded );

	if( options->sendLoadEvent )
	{
		ResourceEvent event;
		event.resource = resource;

		res->resourceTaskEvents.push(event);
	}

	AtomicDecrement(&res->numResourcesQueuedLoad);
	ConditionWakeOne(&res->resourceFinishLoad);

	LogInfo("Loaded resource '%s'", path.c_str());

	Deallocate( AllocatorGetDefault(), options );
}

//-----------------------------------//

} // end namespace