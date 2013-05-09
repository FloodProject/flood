/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_FONT_BITMAP

#include "Engine/Resources/FreeTypeFontLoader.h"
#include "Resources/ResourceManager.h"

#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Stream.h"
#include "Core/Archive.h"
#include "Core/Utilities.h"
#include "Core/Math/Helpers.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(FreeTypeFontLoader, ResourceLoader)
REFLECT_CLASS_END()

//-----------------------------------//

FreeTypeFontLoader::FreeTypeFontLoader()
{
	extensions.push_back("ttf");
}

//-----------------------------------//

bool FreeTypeFontLoader::decode(ResourceLoadOptions& options)
{
	FreeTypeFont* font = (FreeTypeFont*) options.resource;

	StreamRead(options.stream, font->data);

	font->init();

	return true;
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif