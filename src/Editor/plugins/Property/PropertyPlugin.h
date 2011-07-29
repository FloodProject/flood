/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"

#ifdef ENABLE_PLUGIN_PROPERTY

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class PropertyPage;

REFLECT_DECLARE_CLASS(PropertyPlugin)

class PropertyPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(PropertyPlugin)

public:

	PropertyPlugin();

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
	virtual void onComponentSelect( const ComponentPtr& );

	// Component unselection callback.
	virtual void onComponentUnselect( const ComponentPtr& );

	// Resource selection event.
	virtual void onResourceSelect( const ResourcePtr& );

	// Resource unselection event.
	virtual void onResourceUnselect( const ResourcePtr& );

	// Scene load callback.
	virtual void onSceneLoad( const ScenePtr& );

	// Component change callback.
	void onComponentChanged(const ComponentPtr& );

	// Updates the properties.
	void updateProperties(const EntityPtr& entity);

	PropertyPage* propertyPage;
};

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif