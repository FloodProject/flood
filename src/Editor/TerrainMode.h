/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Mode.h"

namespace vapor { namespace editor {

//-----------------------------------//

class TerrainMode : public Mode
{
public:

	TerrainMode( EditorFrame* frame );

	virtual void onModeInit(wxToolBar* toolbar, ModeIdMap& map);
	virtual void onMouseClick(short x, short y);

	void deformTerrain( TerrainPtr terrain, const RayTriangleQueryResult& res );
};

//-----------------------------------//

} } // end namespaces