/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Reflection.h"
#include "Core/Event.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

class Plugin;

/**
 * Responsible for managing all the plugins as well as implementing
 * their lifecycle. Plugins should be registered with the manager
 * and enabled or disabled as needed during runtime.
 */

class API_CORE PluginManager
{
public:

	PluginManager();
	~PluginManager();

	// Gets a plugin from its name.
	Plugin* getPlugin(const String& name);

	// Gets a plugin from its type.
	Plugin* getPluginFromClass(const Class* klass);

	// Registers a new plugin.
	void registerPlugin( Plugin* plugin );

	// Enables the plugin.
	void enablePlugin( Plugin* plugin );

	// Disables the plugin.
	void disablePlugin( Plugin* plugin );

	// Disables all plugins.
	void disablePlugins();

	// Returns if plugin is enabled.
	bool isPluginEnabled( Plugin* plugin ) const;

	// Registers plugins.
	void registerPlugins(const Vector<Plugin*>& plugins);

	// Gets all the registered plugins.
	GETTER(Plugins, Vector<Plugin*>, plugins)

	// Plugin events callback.
	Event1<Plugin*> onPluginEnableEvent;
	Event1<Plugin*> onPluginDisableEvent;

protected:
	
	// Keeps the known plugins.
	Vector<Plugin*> plugins;
};

//-----------------------------------//

NAMESPACE_CORE_END