/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/render/Renderable.h"

namespace vapor { namespace scene {

//-----------------------------------//

/**
 * Pages are pieces of terrain geometry. They are further subdivided in 
 * cells, that are conceptually similar to tiles in 2D games, and allow
 * you to aligned like in an horizontal grid and are identified by integer coordinates.
 */

class VAPOR_API Page : public render::Renderable
{
public:

	Page();
	~Page();

protected:

};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Page );

//-----------------------------------//

} } // end namespaces
