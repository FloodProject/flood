/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/String.h"

NAMESPACE_EDITOR_BEGIN

//---------------------------------------------------------------------//
// Paths
//---------------------------------------------------------------------//

static const char* PluginsFolder = "Plugins/";
static const String MediaFolder( "Assets/" );
static const String CacheFolder( "Cache/" );
static const String ThumbCache( "Thumbs.cache" );
static const int ThumbSize = 256;

static const String HostAddress( "127.0.0.1" );
static const int HostPort = 9999;

// Scene document settings.
static const Color SceneEditClearColor(0.0f, 0.10f, 0.25f);

NAMESPACE_EDITOR_END