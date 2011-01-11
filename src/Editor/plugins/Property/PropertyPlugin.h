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

	// Entity selection callback.
	virtual void onEntitySelect( const EntityPtr& );
	
	// Entity unselection callback.
	virtual void onEntityUnselect( const EntityPtr& );

	// Component selection callback.
	virtual void onComponentSelect( const ComponentPtr& component );

	// Component unselection callback.
	virtual void onComponentUnselect( const ComponentPtr& component );

	// Scene update callback.
	virtual void onSceneUpdate();

	// Field change callback.
	//void onFieldChanged(const Field& field);

	// Component change callback.
	void onComponentChanged(const ComponentPtr& component);

	// Updates the properties.
	void updateProperties();

protected:

	PropertyPage* propertyPage;

	// Selected entity.
	EntityWeakPtr selectedEntity;
};

//-----------------------------------//

} } // end namespaces