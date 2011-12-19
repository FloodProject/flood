/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

FWD_DECL_INTRUSIVE(Program)

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

class Shader;

typedef std::map<const Shader*, ProgramPtr> ShaderProgramsMap;
typedef std::pair<const Shader*, ProgramPtr> ShaderProgramsMapPair;

class ResourceManager;
struct ResourceEvent;

class RenderContext;

/**
 * Manages a set of programs. The rest of the engine, when needing a 
 * program will request the program for a given shader.
 */

class API_GRAPHICS ProgramManager
{
	DECLARE_UNCOPYABLE(ProgramManager)

public:

	ProgramManager();
	~ProgramManager();

	// Gets a program given a shader identifier.
	Program* getProgram( const Shader*, bool precompile = false );

	// Creates a program given a shader.
	Program* createProgram( const Shader* shader );

	// Registers a new program in the manager.
	bool registerProgram( const Shader*, Program* program );

protected:

	// Populates a shader when the text file is loaded.
	void onLoad( const ResourceEvent& event );

	// Reloads a shader when the text file changes.
	void onReload( const ResourceEvent& event );

	// Maps the identifiers to the programs.
	ShaderProgramsMap programs;

	RenderContext* renderContext;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END