/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/ShaderProgramManager.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/RenderContext.h"
#include "Graphics/RenderBackend.h"
#include "Resources/ShaderMaterial.h"
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
#if 0
	ShaderProgramsMap::iterator it;

	for( it = programs.begin(); it != programs.end(); it++ )
	{
		ShaderProgram* program = it->second.get();
		if( !program ) continue;

		assert( ReferenceGetCount(program) == 1 );
	}
#endif

	GetResourceManager()->onResourceLoaded.Disconnect( this, &ProgramManager::onLoad );
	GetResourceManager()->onResourceReloaded.Disconnect( this, &ProgramManager::onReload );
}

//-----------------------------------//

ShaderProgram* ProgramManager::createProgram( const ShaderMaterial* shader )
{
	// If the program was not yet found, then we need to create it.
	ShaderProgram* program = GetRenderDevice()->getBackend()->createProgram();
	
	program->getVertexShader()->setText( shader->getVertexSource() );
	program->getFragmentShader()->setText( shader->getFragmentSource() );

	// Force the recompilation of all shaders in the program.
	program->forceRecompile();

	return program;
}

//-----------------------------------//

ShaderProgram* ProgramManager::getProgram( const ShaderMaterial* shader, bool precompile )
{
	if( !shader ) return nullptr;

	ShaderProgramsMap::iterator it = programs.find(shader);

	if( it != programs.end() )
	{
		ShaderProgram* program = it->second.get();
		return program;
	}

	ShaderProgram* program = createProgram(shader);
	registerProgram(shader, program);

	return program;
}

//-----------------------------------//

bool ProgramManager::registerProgram( const ShaderMaterial* shader, ShaderProgram* program )
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

	ShaderMaterial* shader = (ShaderMaterial*) resource;
	getProgram(shader);

	LogInfo("Loaded shader '%s'", shader->getPath().c_str() );
}

//-----------------------------------//

void ProgramManager::onReload( const ResourceEvent& event )
{
	Resource* resource = event.handle.Resolve();

	if( resource->getResourceGroup() != ResourceGroup::Shaders )
		return;

	ShaderMaterial* oldShader = (ShaderMaterial*) event.oldResource;

	#pragma TODO("Handle reloading of unregistered resources")
	
	ShaderProgramsMap::iterator it = programs.find(oldShader);
	assert( it != programs.end() );
	
	ShaderProgramPtr program = it->second; // We need to hold the ref.
	programs.erase(it);

	ShaderMaterial* shader = (ShaderMaterial*) event.resource;
	programs[shader] = program;

	LogDebug( "Reloading shader '%s'", shader->getPath().c_str() );

	program->getVertexShader()->setText( shader->getVertexSource() );
	program->getFragmentShader()->setText( shader->getFragmentSource() );

	// Force the recompilation of all shader programs.
	program->forceRecompile();
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END