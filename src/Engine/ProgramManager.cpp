/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/ProgramManager.h"
#include "vapor/render/GLSL_Program.h"
#include "vapor/resources/GLSL_Text.h"
#include "vapor/resources/ResourceManager.h"
#include "vapor/Utilities.h"

namespace vapor {

//-----------------------------------//

ProgramManager::ProgramManager( ResourceManager* resourceManager )
	: rm( resourceManager )
{
	assert( rm != nullptr );
	
	rm->onResourceLoaded.Connect( this, &ProgramManager::onLoad );
	rm->onResourceReloaded.Connect( this, &ProgramManager::onReload );
}

//-----------------------------------//

ProgramManager::~ProgramManager()
{
	assert( rm != nullptr );
	
	rm->onResourceLoaded.Disconnect( this, &ProgramManager::onLoad );
	rm->onResourceReloaded.Disconnect( this, &ProgramManager::onReload );
}

//-----------------------------------//

ProgramPtr ProgramManager::getProgram( const std::string& name )
{
	if( programs.find(name) == programs.end() )
	{
		#pragma TODO("Get GLSL fallback programs working")
		
		Log::warn( "Could not locate '%s'", name.c_str() );
		return nullptr;
	}

	return programs[name];
}

//-----------------------------------//

bool ProgramManager::registerProgram( const std::string& name, const ProgramPtr& program )
{
	assert( program != nullptr );
	
	if( programs.find(name) != programs.end() )
	{
		Log::warn( "Shader '%s' already registered", name.c_str() );
		return false;
	}

	programs[name] = program;
	return true;
}

//-----------------------------------//

void ProgramManager::onLoad( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Shaders )
		return;
	
	const TextPtr& text = RefCast<Text>( evt.resource );
	const GLSL_TextPtr& glsl = RefCast<GLSL_Text>( text );
	
	const GLSL_ProgramPtr program = new GLSL_Program(glsl);
	registerProgram( glsl->getBasePath(), program );
}

//-----------------------------------//

void ProgramManager::onReload( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Shaders )
		return;

	const TextPtr& text = RefCast<Text>( evt.resource );
	std::string base( String::toLowerCase( text->getBasePath() ) );

	assert( programs.find(base) != programs.end() );

	Log::debug( "Reloading shader '%s'", evt.resource->getPath().c_str() );

	ProgramPtr program = programs[base];
	program->updateShadersText();
}

//-----------------------------------//

} // end namespace