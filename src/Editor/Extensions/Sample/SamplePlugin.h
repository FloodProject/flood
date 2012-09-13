/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(SamplePlugin)

class SamplePlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(SamplePlugin)

public:

	SamplePlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();
};

//-----------------------------------//

NAMESPACE_EDITOR_END