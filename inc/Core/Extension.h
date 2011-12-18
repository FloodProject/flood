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
	// Name of this extension.
	const char* name;

	// Description of this extension.
	const char* description;

	// Priority of the extension.
	int priority;
};

//-----------------------------------//

/**
 * Extensions are clearly defined units of extensibility to applications.
 * Typically an application will create the extension points and provide
 * some kind of runtime support for loading the extensions from a plugin.
 */

REFLECT_DECLARE_CLASS(Extension)

class API_CORE Extension : public Object
{
	REFLECT_DECLARE_OBJECT(Extension)

public:

	Extension();
	virtual ~Extension();

	// Gets metadata about this extension.
	virtual ExtensionMetadata* getMetadata() = 0;

	// Extension initialize callback.
	virtual void onInit();

	// Extension cleanup callback.
	virtual void onCleanup();
};

//-----------------------------------//

NAMESPACE_CORE_END