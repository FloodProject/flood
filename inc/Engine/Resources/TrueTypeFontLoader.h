/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Resources/ResourceLoader.h"	
#include "Engine/Resources/TrueTypeFont.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//


REFLECT_DECLARE_CLASS(TrueTypeFontLoader)

class API_ENGINE TrueTypeFontLoader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(TrueTypeFontLoader)

public:

	TrueTypeFontLoader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(TrueTypeFont)

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Font)

	// Decode a font definition.
	virtual bool decode(ResourceLoadOptions&) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "FONTS")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Fonts)
};

//-----------------------------------//

NAMESPACE_ENGINE_END
