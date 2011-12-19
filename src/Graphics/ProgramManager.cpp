/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/ProgramManager.h"
#include "Graphics/Program.h"
#include "Graphics/RenderContext.h"
#include "Graphics/RenderBackend.h"
#include "Resources/Shader.h"
#include "Resources/ResourceManager.h"
#include "Core/Utilities.h"

NAMESPACE_GRAPHICS_BEGIN

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
		Program* program = it->second.get();
		if( !program ) continue;

		assert( ReferenceGetCount(program) == 1 );
	}

	GetResourceManager()->onResourceLoaded.Disconnect( this, &ProgramManager::onLoad );
	GetResourceManager()->onResourceReloaded.Disconnect( this, &ProgramManager::onReload );
}

//-----------------------------------//

Program* ProgramManager::createProgram( const Shader* shader )
{
	// If the program was not yet found, then we need to create it.
	Program* program = renderContext->backend->createProgram();
	
	program->getVertexShader()->setText( shader->getVertexSource() );
	program->getFragmentShader()->setText( shader->getFragmentSource() );

	// Force the recompilation of all shader programs.
	program->forceRecompile();

	return program;
}

//-----------------------------------//

Program* ProgramManager::getProgram( const Shader* shader, bool precompile )
{
	if( !shader ) return nullptr;

	ShaderProgramsMap::iterator it = programs.find(shader);

	if( it != programs.end() )
	{
		Program* program = it->second.get();
		return program;
	}

	Program* program = createProgram(shader);
	registerProgram(shader, program);

	return program;
}

//-----------------------------------//

bool ProgramManager::registerProgram( const Shader* shader, Program* program )
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
	
	Program* program = it->second.get();
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

NAMESPACE_GRAPHICS_END