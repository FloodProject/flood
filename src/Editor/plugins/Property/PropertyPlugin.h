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

//-----------------------------------//

class PropertyPage;

//-----------------------------------//

class PropertyPlugin : public Plugin
{
public:

	PropertyPlugin( EditorFrame* frame );

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Node selection callback.
	virtual void onNodeSelect( const NodePtr& );
	
	// Node unselection callback.
	virtual void onNodeUnselect( const NodePtr& );

	// Scene update callback.
	virtual void onSceneUpdate();

	// Field change callback.
	void onFieldChanged(const Field& field);

protected:

	PropertyPage* propertyPage;
	int iconProperty;

	// Selected node.
	NodeWeakPtr selectedNode;
};

//-----------------------------------//

} } // end namespaces