/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Reflection.h"
#include "Core/Object.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct API_CORE PluginMetadata
{
	PluginMetadata();

	// Name of this plugin.
	String name;

	// Description of this plugin.
	String description;

	// Author of this plugin.
	String author;

	// Version of this plugin.
	String version;

	// Enabled by default.
	bool startEnabled;

	// Startup priority.
	int priority;
};

//-----------------------------------//

class PluginManager;

/**
 * Plugins provide units of extensibility to applications.
 */

API_CORE REFLECT_DECLARE_CLASS(Plugin)

class API_CORE Plugin : public Object
{
	REFLECT_DECLARE_OBJECT(Plugin)

	friend class PluginManager;

public:

	Plugin();
	virtual ~Plugin();

	// Gets if plugin is enabled.
	bool isEnabled() const;

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata() = 0;

	// Plugin callbacks.
	virtual void onPluginEnable() {}
	virtual void onPluginDisable() {}
	virtual void onPluginUpdate() {}

protected:

	// Helper method to disable plugins.
	virtual void doPluginDisable();
	
	// Keeps track if plugin is enabled.
	bool enabled;
};

// Sorts plugins by priority.
API_CORE void PluginsSortByPriority(Vector<Plugin*>& plugins);

//-----------------------------------//

NAMESPACE_CORE_END