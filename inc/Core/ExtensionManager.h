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

	Extension();
	virtual ~Extension();

	// Gets if extension is enabled.
	bool isEnabled() const;

	// Gets metadata about this extesion.
	virtual ExtensionMetadata getMetadata() = 0;

	// Extension callbacks.
	virtual void onExtensionEnable() {}
	virtual void onExtensionDisable() {}

protected:

	// Keeps track if extension is enabled.
	bool enabled;
};

//-----------------------------------//

NAMESPACE_CORE_END