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

#include "vapor/scene/Node.h"
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
	foreach( Page* const page, pages )
		delete page;
}

//-----------------------------------//

void PageManager::update( double delta )
{
	const CameraPtr& camera = weakCamera.lock();

	// No associated current camera.
	if(!camera)
		return;

	const NodePtr& nodeCamera = camera->getNode();
	assert( nodeCamera != nullptr );

	const TransformPtr& transform = nodeCamera->getTransform();
	assert( transform != nullptr );

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