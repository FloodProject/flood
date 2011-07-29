/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class HierarchyPane;

REFLECT_DECLARE_CLASS(HierarchyPlugin)

class HierarchyPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(HierarchyPlugin)

public:

	HierarchyPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

protected:

	HierarchyPane* hierarchyPane;
};

//-----------------------------------//

NAMESPACE_EDITOR_END