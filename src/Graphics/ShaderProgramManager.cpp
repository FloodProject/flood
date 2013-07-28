/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/ShaderProgramManager.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/RenderContext.h"
#include "Graphics/RenderBackend.h"
#include "Graphics/Resources/ShaderMaterial.h"
#include "Resources/ResourceManager.h"
#include "Core/Utilities.h"
#include "Core/Containers/Hash.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

ProgramManager::ProgramManager(RenderBackend* backend)
	: backend(backend)
	, programs(*AllocatorGetHeap())
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
	ShaderProgram* program = backend->createProgram();
	
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

	auto it = programs.get((uint64)shader, nullptr);
	if(!it)
	{
		auto prog = createProgram(shader);
		registerProgram(shader, prog);
		it.reset(prog);
	}

	return it.get();
}

//-----------------------------------//

bool ProgramManager::registerProgram( const ShaderMaterial* shader, ShaderProgram* program )
{
	if(programs.has((uint64)shader))
	{
		LogWarn( "Shader '%s' already registered", shader->getPath().c_str() );
		return false;
	}

	programs.set((uint64)shader, ShaderProgramPtr(program));
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
	ShaderMaterial* shader = (ShaderMaterial*) event.resource;

	#pragma TODO("Handle reloading of unregistered resources")

	auto prog = programs.get((uint64)oldShader, nullptr);
	assert( prog.get() != nullptr );
	
	programs.remove((uint64)oldShader);
	programs.set((uint64)shader, prog);

	LogDebug( "Reloading shader '%s'", shader->getPath().c_str() );

	prog->getVertexShader()->setText( shader->getVertexSource() );
	prog->getFragmentShader()->setText( shader->getFragmentSource() );

	// Force the recompilation of all shader programs.
	prog->forceRecompile();
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END