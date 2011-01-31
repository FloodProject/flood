/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "render/Program.h"

FWD_DECL_INTRUSIVE(Text)

namespace vapor {

//-----------------------------------//

typedef std::map<std::string, ProgramPtr> ProgramsMap;
typedef std::pair<const std::string&, ProgramPtr> ProgramsPair;

class ResourceManager;
struct ResourceEvent;

/**
 * Manages a set of programs. The rest of the engine, when needing a 
 * program will request the program for a given shader.
 */

class VAPOR_API ProgramManager
{
	DECLARE_UNCOPYABLE(ProgramManager)

public:

	ProgramManager();
	~ProgramManager();

	// Gets a program given a name identifier.
	ProgramPtr getProgram( const std::string& program );

	// Registers a new program in the manager.
	bool registerProgram( const std::string& name, const ProgramPtr& program );

	// Creates a program given their shader text.
	ProgramPtr createProgram( const TextPtr& );

protected:

	// Populates a shader when the text file is loaded.
	void onLoad( const ResourceEvent& evt );

	// Reloads a shader when the text file changes.
	void onReload( const ResourceEvent& evt );

	// Maps the identifiers to the programs.
	ProgramsMap programs;
};

//-----------------------------------//

} // end namespace