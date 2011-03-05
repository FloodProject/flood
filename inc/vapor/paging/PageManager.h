/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/Subsystem.h"
#include "vapor/math/Vector2.h"
#include "vapor/math/Vector3.h"
#include "Event.h"

FWD_DECL_SHARED(Camera)
FWD_DECL_SHARED_WEAK(Camera)

namespace vapor {

//-----------------------------------//

class Page;

//-----------------------------------//

struct PageEvent
{
	Page* page;
	Vector2i pos;
};

//-----------------------------------//

/**
 * This class is responsible for handling all the pages
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
	Vector2i page;

	// Converts world coords to page location.
	Vector2i convertWorldToPage(const Vector3& pos);

	// Holds the size of pages.
	uint pageSize;

	// Current camera.
	CameraWeakPtr weakCamera;

	// Holds the loaded pages.
	std::vector<Page*> pages;
};

//-----------------------------------//

} // end namespace