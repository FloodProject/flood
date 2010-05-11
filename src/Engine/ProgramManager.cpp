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
#include "vapor/Engine.h"

namespace vapor { namespace render {

using namespace vapor::resources;

//-----------------------------------//

ProgramManager::ProgramManager()
	: rm( Engine::getInstance().getResourceManager() )
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

	const ProgramPtr& program = programs[lower];

	if( !program )
	{
		// use a fallback default program
		// TODO: bundle fallback program in engine
		
		warn( "program", "Could not locate '%s'", name.c_str() );
	}

	return program;
}

//-----------------------------------//

bool ProgramManager::registerProgram( const std::string& name, const ProgramPtr& program )
{
	assert( program != nullptr );
	
	std::string lower( String::toLowerCase(name) );

	// TODO: Better error handling.
	if( programs.find(lower) != programs.end() )
	{
		warn( "program", "Shader '%s' already registered", name.c_str() );
		return false;
	}

	programs[lower] = program;
	return true;
}

//-----------------------------------//

void ProgramManager::createShaders( const TextPtr& text )
{
	const GLSL_TextPtr& glsl = RESOURCE_SMART_PTR_CAST<GLSL_Text>( text );

	const GLSL_ShaderPtr& vertex = new GLSL_Shader();
	vertex->setType( ShaderType::Vertex );
	vertex->setText( glsl->getVertexSource() );

	const GLSL_ShaderPtr& fragment = new GLSL_Shader();
	fragment->setType( ShaderType::Fragment );
	fragment->setText( glsl->getFragmentSource() );

	const GLSL_ProgramPtr program = new GLSL_Program( vertex, fragment );
	program->setText( glsl );
	
	registerProgram( glsl->getBaseURI(), program );
}

//-----------------------------------//

void ProgramManager::onLoad( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Shaders )
		return;
	
	const TextPtr& text = RESOURCE_SMART_PTR_CAST<Text>( evt.resource );

	createShaders( text );
}

//-----------------------------------//

void ProgramManager::onReload( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Shaders )
		return;
}

//-----------------------------------//

} } // end namespaces