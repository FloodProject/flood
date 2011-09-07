/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Render/Buffer.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

typedef std::map<uint32, BufferPtr> BuffersMap;
typedef std::pair<const String&, BufferPtr> BuffersPair;

class ResourceManager;
struct ResourceEvent;

/**
 * Manages a set of buffers.
 */

class API_RENDER BufferManager
{
	DECLARE_UNCOPYABLE(BufferManager)

public:

	BufferManager();
	~BufferManager();

protected:

	// Populates a shader when the text file is loaded.
	void onLoad( const ResourceEvent& evt );

	// Reloads a shader when the text file changes.
	void onReload( const ResourceEvent& evt );

	// Maps the identifiers to the programs.
	BuffersMap programs;
};

//-----------------------------------//

NAMESPACE_ENGINE_END