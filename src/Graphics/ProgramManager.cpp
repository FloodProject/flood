/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Graphics/ProgramManager.h"
#include "Graphics/Program.h"
#include "Graphics/GLSL_Program.h"
#include "Resources/Shader.h"
#include "Resources/GLSL_Shader.h"
#include "Resources/ResourceManager.h"
#include "Core/Utilities.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

ProgramManager::ProgramManager()
{
	GetResourceManager()->onResourceLoaded.Connect( this, &ProgramManager::onLoad );
	GetResourceManager()->onResourceReloaded.Connect( this, &ProgramManager::onReload );
}

//-----------------------------------//

ProgramManager::~ProgramManager()
{
	ShaderProgramsMap::iterator it;

	for( it = programs.begin(); it != programs.end(); it++ )
	{
		const GLSL_ProgramPtr& program = it->second;
		if( !program ) continue;

		assert( ReferenceGetCount( program.get() ) == 1 );
	}

	GetResourceManager()->onResourceLoaded.Disconnect( this, &ProgramManager::onLoad );
	GetResourceManager()->onResourceReloaded.Disconnect( this, &ProgramManager::onReload );
}

//-----------------------------------//

ProgramPtr ProgramManager::getProgram( const Shader* shader, bool precompile )
{
	if( !shader ) return nullptr;

	ShaderProgramsMap::iterator it = programs.find(shader);

	if( it != programs.end() )
	{
		const GLSL_ProgramPtr& program = it->second;
		return program;
	}

	GLSL_ProgramPtr program = createProgram(shader);
	registerProgram(shader, program);

	return program;
}

//-----------------------------------//

GLSL_ProgramPtr ProgramManager::createProgram( const Shader* shader )
{
	// If the program was not yet found, then we need to create it.
	GLSL_ProgramPtr program = AllocateThis(GLSL_Program);
	
	program->getVertexShader()->setText( shader->getVertexSource() );
	program->getFragmentShader()->setText( shader->getFragmentSource() );

	// Force the recompilation of all shader programs.
	program->forceRecompile();

	return program;
}

#pragma TODO("Get GLSL fallback programs working")

//-----------------------------------//

bool ProgramManager::registerProgram( const Shader* shader, const GLSL_ProgramPtr& program )
{
	if( programs.find(shader) != programs.end() )
	{
		LogWarn( "Shader '%s' already registered", shader->getPath().c_str() );
		return false;
	}

	programs[shader] = program;
	return true;
}

//-----------------------------------//

void ProgramManager::onLoad( const ResourceEvent& event )
{
	Resource* resource = event.handle.Resolve();

	if( resource->getResourceGroup() != ResourceGroup::Shaders )
		return;

	Shader* shader = (Shader*) resource;
	getProgram(shader);

	LogInfo("Loaded shader '%s'", shader->getPath().c_str() );
}

//-----------------------------------//

void ProgramManager::onReload( const ResourceEvent& event )
{
	Resource* resource = event.handle.Resolve();

	if( resource->getResourceGroup() != ResourceGroup::Shaders )
		return;

	Shader* oldShader = (Shader*) event.oldResource;

	#pragma TODO("Handle reloading of unregistered resources")
	
	ShaderProgramsMap::iterator it = programs.find(oldShader);
	assert( it != programs.end() );
	
	GLSL_ProgramPtr program = it->second;
	programs.erase(it);

	Shader* shader = (Shader*) event.resource;
	programs[shader] = program;

	LogDebug( "Reloading shader '%s'", shader->getPath().c_str() );

	program->getVertexShader()->setText( shader->getVertexSource() );
	program->getFragmentShader()->setText( shader->getFragmentSource() );

	// Force the recompilation of all shader programs.
	program->forceRecompile();
}

//-----------------------------------//

NAMESPACE_ENGINE_END