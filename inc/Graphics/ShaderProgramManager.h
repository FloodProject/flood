/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Graphics/API.h"
#include "Core/References.h"

FWD_DECL_INTRUSIVE(ShaderProgram)

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

class ShaderMaterial;

typedef std::map<const ShaderMaterial*, ShaderProgramPtr> ShaderProgramsMap;
typedef std::pair<const ShaderMaterial*, ShaderProgramPtr> ShaderProgramsMapPair;

class ResourceManager;
struct ResourceEvent;

class RenderContext;
class RenderBackend;

/**
 * Manages a set of programs. The rest of the engine, when needing a 
 * program will request the program for a given shader.
 */

class API_GRAPHICS ProgramManager
{
	DECLARE_UNCOPYABLE(ProgramManager)

public:

	ProgramManager(RenderBackend* backend);
	~ProgramManager();

	// Gets a program given a shader identifier.
	ShaderProgram* getProgram( const ShaderMaterial*, bool precompile = false );

	// Creates a program given a shader.
	ShaderProgram* createProgram( const ShaderMaterial* shader );

	// Registers a new program in the manager.
	bool registerProgram( const ShaderMaterial*, ShaderProgram* program );

protected:

	// Populates a shader when the text file is loaded.
	void onLoad( const ResourceEvent& event );

	// Reloads a shader when the text file changes.
	void onReload( const ResourceEvent& event );

	// Maps the identifiers to the programs.
	ShaderProgramsMap programs;

	RenderBackend* backend;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END