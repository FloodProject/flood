/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/paging/PageManager.h"
#include "vapor/paging/Page.h"

#include "vapor/scene/Entity.h"
#include "vapor/scene/Transform.h"
#include "vapor/scene/Camera.h"

namespace vapor {

bool loaded[100][100];

//-----------------------------------//

PageManager::PageManager( uint pageSize )
	: pageSize(pageSize)
{ }

//-----------------------------------//

PageManager::PageManager( uint pageSize, CameraWeakPtr weakCamera )
	: pageSize(pageSize)
	, weakCamera(weakCamera)
{ }

//-----------------------------------//

PageManager::~PageManager()
{
	for( uint i = 0; i < pages.size(); i++ )
	{
		Page* const page = pages[i];
		delete page;
	}
}

//-----------------------------------//

void PageManager::update( float delta )
{
	const CameraPtr& camera = weakCamera.lock();

	// No associated current camera.
	if(!camera)
		return;

	Entity* entity = camera->getEntity();
	const TransformPtr& transform = entity->getTransform();

	Vector3 pos = transform->getPosition();
	Vector2i newPage = convertWorldToPage(pos);

	if( page != newPage )
	{
		page = newPage;

		PageEvent event;
		event.pos = page;
	
		onPageSwitch(event);

		if(!loaded[page.x][page.y])
		{
			loaded[page.x][page.y] = true;

			onPageLoading(event);
		}
	}
}

//-----------------------------------//

Vector2i PageManager::convertWorldToPage(const Vector3& pos)
{
	int x = floor(pos.x / pageSize); 
	int y = floor(pos.z / pageSize);

	return Vector2i(x, y);
}

//-----------------------------------//

} // end namespace