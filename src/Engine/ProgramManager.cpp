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

namespace vapor {

//-----------------------------------//

ProgramManager::ProgramManager( ResourceManager* resourceManager )
	: rm( resourceManager )
{
	assert( rm != nullptr );
	
	rm->onResourceLoaded += fd::bind( &ProgramManager::onLoad, this );
	rm->onResourceReloaded += fd::bind( &ProgramManager::onReload, this );
}

//-----------------------------------//

ProgramManager::~ProgramManager()
{
	assert( rm != nullptr );
	
	rm->onResourceLoaded -= fd::bind( &ProgramManager::onLoad, this );
	rm->onResourceReloaded -= fd::bind( &ProgramManager::onReload, this );
}

//-----------------------------------//

ProgramPtr ProgramManager::getProgram( const std::string& name )
{
	std::string lower( String::toLowerCase(name) );

	if( programs.find(lower) == programs.end() )
	{
		#pragma TODO("Get GLSL fallback programs working")
		
		warn( "program", "Could not locate '%s'", name.c_str() );
		return ProgramPtr();
	}

	return programs[lower];
}

//-----------------------------------//

bool ProgramManager::registerProgram( const std::string& name, const ProgramPtr& program )
{
	assert( program != nullptr );
	
	std::string lower( String::toLowerCase(name) );

	if( programs.find(lower) != programs.end() )
	{
		warn( "program", "Shader '%s' already registered", name.c_str() );
		return false;
	}

	programs[lower] = program;
	return true;
}

//-----------------------------------//

void ProgramManager::onLoad( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Shaders )
		return;
	
	const TextPtr& text = RESOURCE_SMART_PTR_CAST<Text>( evt.resource );
	const GLSL_TextPtr& glsl = RESOURCE_SMART_PTR_CAST<GLSL_Text>( text );
	
	const GLSL_ProgramPtr program = new GLSL_Program(glsl);
	registerProgram( glsl->getBasePath(), program );
}

//-----------------------------------//

void ProgramManager::onReload( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Shaders )
		return;

	const TextPtr& text = RESOURCE_SMART_PTR_CAST<Text>( evt.resource );
	std::string base( String::toLowerCase( text->getBasePath() ) );

	assert( programs.find(base) != programs.end() );

	debug( "Reloading shader '%s'", evt.resource->getPath().c_str() );

	ProgramPtr program = programs[base];
	program->updateShadersText();
}

//-----------------------------------//

} // end namespace