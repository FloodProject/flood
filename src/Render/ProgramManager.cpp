/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "Render/ProgramManager.h"
#include "Render/GLSL_Program.h"
#include "Resources/GLSL_Text.h"
#include "Resources/ResourceManager.h"
#include "Utilities.h"

namespace vapor {

//-----------------------------------//

ProgramManager::ProgramManager()
{
	GetResourceManager()->onResourceLoaded.Connect( this, &ProgramManager::onLoad );
	GetResourceManager()->onResourceReloaded.Connect( this, &ProgramManager::onReload );
}

//-----------------------------------//

ProgramManager::~ProgramManager()
{
	GetResourceManager()->onResourceLoaded.Disconnect( this, &ProgramManager::onLoad );
	GetResourceManager()->onResourceReloaded.Disconnect( this, &ProgramManager::onReload );
}

//-----------------------------------//

ProgramPtr ProgramManager::getProgram( const std::string& name )
{
	if( programs.find(name) != programs.end() )
		return programs[name];

	ResourceManager* res = GetResourceManager();

	// Tries to load the shader. The program is created in onLoad.
	ResourceLoadOptions options;
	options.name = name;
	options.group = ResourceGroup::Shaders;
	options.asynchronousLoading = false;

	if( !res->loadResource(options) )
		return nullptr;

	return programs[name];
}

#pragma TODO("Get GLSL fallback programs working")

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

ProgramPtr ProgramManager::createProgram(const TextPtr& text)
{
	const GLSL_TextPtr& gtext = RefCast<GLSL_Text>(text);
	GLSL_ProgramPtr program = new GLSL_Program(gtext);
	
	return program;
}

//-----------------------------------//

void ProgramManager::onLoad( const ResourceEvent& event )
{
	if( event.resource->getResourceGroup() != ResourceGroup::Shaders )
		return;
	
	const TextPtr& text = RefCast<Text>( event.resource );
	const ProgramPtr& program = createProgram(text);
	registerProgram( text->getBasePath(), program );
}

//-----------------------------------//

void ProgramManager::onReload( const ResourceEvent& event )
{
	if( event.resource->getResourceGroup() != ResourceGroup::Shaders )
		return;

	const TextPtr& text = RefCast<Text>( event.resource );
	const std::string& base = text->getBasePath();

	ProgramPtr program = programs[base];

	if(program)
	{
		Log::debug( "Reloading shader '%s'", event.resource->getPath().c_str() );
		program->updateShadersText();
	}
}

//-----------------------------------//

} // end namespace