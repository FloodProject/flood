/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceLoader.h"

#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Stream.h"
#include "Core/Archive.h"
#include "Core/Utilities.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

void ResourceTaskRun(Task* task)
{
	ResourceLoadOptions* options = (ResourceLoadOptions*) task->userdata;

	Stream* stream = options->stream;
	Resource* resource = options->resource;
	const Path& path = resource->getPath();
	
	ResourceManager* res = GetResourceManager();
	ResourceLoader* loader = res->findLoader( PathGetFileExtension(path) );

	bool decoded = loader->decode(*options);

	if( !decoded )
	{
		resource->setStatus( ResourceStatus::Error );
		LogWarn("Error decoding resource '%s'", path.c_str());
		goto cleanup;
	}

	resource->setStatus( ResourceStatus::Loaded );

	LogInfo("Loaded resource '%s'", path.c_str());

	if( options->sendLoadEvent )
	{
		ResourceEvent event;
		event.resource = resource;
		res->resourceEvents.push_back(event);
	}

cleanup:

	res->numResourcesQueuedLoad.decrement();
	res->resourceFinishLoad->wakeOne();

	if( !options->keepStreamOpen )
		Deallocate(stream);

	Deallocate(options);
}

//-----------------------------------//

NAMESPACE_RESOURCES_END