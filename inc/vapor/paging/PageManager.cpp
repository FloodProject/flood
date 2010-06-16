/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/paging/PageManager.h"
#include "vapor/scene/Camera.h"

namespace vapor {

//-----------------------------------//

PageManager::PageManager( const CameraWeakPtr& weakCamera )
	: camera(weakCamera)
{ }

//-----------------------------------//

void PageManager::update( double delta )
{

}

//-----------------------------------//

} // end namespace