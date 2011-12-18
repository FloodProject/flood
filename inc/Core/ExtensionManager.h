/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Reflection.h"
#include "Core/Object.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct API_CORE ExtensionMetadata
{
	ExtensionMetadata();

	// Name of this extension.
	String name;

	// Description of this plugin.
	String description;
};

//-----------------------------------//

/**
 * Extensions are clearly defined units of extensibility to applications.
 * Typically an application will create the extension points and provide
 * some kind of runtime support for loading the extensions from a plugin.
 */

REFLECT_DECLARE_CLASS(Plugin)

class API_CORE Extension : public Object
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

	// Sorts plugins by priority.
	static void sortByPriority(std::vector<Plugin*>& plugins);

protected:

	// Helper method to disable plugins.
	virtual void doPluginDisable();
	
	// Keeps track if plugin is enabled.
	bool enabled;
};

//-----------------------------------//

NAMESPACE_CORE_END