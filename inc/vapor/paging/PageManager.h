/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/Subsystem.h"

FWD_DECL_TYPEDEF_SHARED_WEAK(Camera)
FWD_DECL_TYPEDEF_PTR(Page)

namespace vapor {

//-----------------------------------//

struct PageEvent
{
	
};

//-----------------------------------//

/**
 * This class is responsible for handling all the pages
 */

class VAPOR_API PageManager : public Subsystem
{
public:

	PageManager( const CameraWeakPtr& camera );

	// Checks if paging is neeed.
	virtual void update( double delta );

	// Paging is needed.
	fd::delegate<void(const PageEvent&)> PageLoading;
	fd::delegate<void(const PageEvent&)> PageLoaded;
	fd::delegate<void(const PageEvent&)> PageRemoved;

protected:

	CameraWeakPtr camera;
	std::vector<PagePtr> pages;
};

TYPEDEF_PTR(Page)

//-----------------------------------//

} // end namespace