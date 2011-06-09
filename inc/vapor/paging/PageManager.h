/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Subsystem.h"
#include "Math/Vector.h"
#include "Math/Vector.h"
#include "Event.h"

#if 0

FWD_DECL_INTRUSIVE(Camera)

namespace vapor {

//-----------------------------------//

class Page;

struct PageEvent
{
	Page* page;
	Vector2 pos;
};

/**
 * This class is responsible for handling all the pages.
 */

class VAPOR_API PageManager : public Subsystem
{
public:

	PageManager( uint pageSize );
	PageManager( uint pageSize, CameraWeakPtr weakCamera );
	~PageManager();

	// Loads a given page.
	Page* loadPage( uint x, uint y );

	// Sets the current camera. 
	SETTER(Camera, const CameraPtr&, weakCamera)

	// Checks if paging is needed.
	virtual void update( float delta );

	// Paging events.
	Event1<const PageEvent&> onPageSwitch;
	Event1<const PageEvent&> onPageLoading;
	Event1<const PageEvent&> onPageLoaded;
	Event1<const PageEvent&> onPageRemoved;

protected:

	// Current page.
	Vector2 page;

	// Converts world coords to page location.
	Vector2 convertWorldToPage(const Vector3& pos);

	// Holds the size of pages.
	uint pageSize;

	// Current camera.
	CameraWeakPtr weakCamera;

	// Holds the loaded pages.
	std::vector<Page*> pages;
};

//-----------------------------------//

} // end namespace

#endif