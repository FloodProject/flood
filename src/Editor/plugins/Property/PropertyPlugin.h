/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"
#include <wx/propgrid/property.h>

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

protected:

	// Populates properties on the grid.
	void showNodeProperties( const NodePtr& );

	// Appends the type fields to the property grid.
	void appendObjectFields(const Class& type, void* object);

	// Creates a new property for a primitive type.
	wxPGProperty* createPrimitiveProperty(const Field& field, void* object);

	// Creates a new property for an enum type.
	wxPGProperty* createEnumProperty(const Field& field, void* object);

	// Formats the given string to a readable version for display.
	wxString convertToReadable(const wxString& str);

	PropertyPage* propertyPage;
	int iconProperty;

	// Selected node.
	NodeWeakPtr selectedNode;
};

//-----------------------------------//

} } // end namespaces