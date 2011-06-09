/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Paging/PageManager.h"
#include "Paging/Page.h"

#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"

#if 0

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
	for( size_t i = 0; i < pages.size(); i++ )
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
	Vector2 newPage = convertWorldToPage(pos);

	if( page != newPage )
	{
		page = newPage;

		PageEvent event;
		event.pos = page;
	
		onPageSwitch(event);

		if(!loaded[(int)page.x][(int)page.y])
		{
			loaded[(int)page.x][(int)page.y] = true;

			onPageLoading(event);
		}
	}
}

//-----------------------------------//

Vector2 PageManager::convertWorldToPage(const Vector3& pos)
{
	int x = floor(pos.x / pageSize); 
	int y = floor(pos.z / pageSize);

	return Vector2(x, y);
}

//-----------------------------------//

} // end namespace

#endif