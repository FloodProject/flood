/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"

namespace vapor { namespace editor {

class CameraControls;

//-----------------------------------//

class CameraPlugin : public Plugin
{
public:

	CameraPlugin( EditorFrame* frame );

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable( wxToolBar* );

	// Plugin disable callback.
	virtual void onPluginDisable( wxToolBar* );

protected:

	CameraControls* cameraControls;
};

//-----------------------------------//

} } // end namespaces