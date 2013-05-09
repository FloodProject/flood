/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Resources/ResourceLoader.h"	
#include "Engine/Resources/FreeTypeFont.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//


REFLECT_DECLARE_CLASS(FreeTypeFontLoader)

class API_ENGINE FreeTypeFontLoader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(FreeTypeFontLoader)

public:

	FreeTypeFontLoader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(FreeTypeFont)

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
