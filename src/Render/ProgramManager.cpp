/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/ProgramManager.h"
#include "Render/GLSL_Program.h"
#include "Resources/GLSL_Text.h"
#include "Resources/ResourceManager.h"
#include "Core/Utilities.h"

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

ProgramPtr ProgramManager::getProgram( const String& name, bool precompile )
{
	if( programs.find(name) != programs.end() )
		return programs[name];

	ResourceManager* res = GetResourceManager();

	// Tries to load the shader. The program is created in onLoad.
	ResourceLoadOptions options;
	options.name = name;
	options.group = ResourceGroup::Shaders;
	options.asynchronousLoad = false;

	if( !res->loadResource(options) )
		return nullptr;

	const ProgramPtr& program = programs[name];
	
	if( precompile && program )
		program->link();

	return program;
}

#pragma TODO("Get GLSL fallback programs working")

//-----------------------------------//

bool ProgramManager::registerProgram( const String& name, const ProgramPtr& program )
{
	if( programs.find(name) != programs.end() )
	{
		LogWarn( "Shader '%s' already registered", name.c_str() );
		return false;
	}

	programs[name] = program;
	return true;
}

//-----------------------------------//

ProgramPtr ProgramManager::createProgram(Text* text)
{
	GLSL_Text* gtext = (GLSL_Text*) text;
	GLSL_ProgramPtr program = Allocate(GLSL_Program, AllocatorGetHeap(), gtext);
	
	return program;
}

//-----------------------------------//

void ProgramManager::onLoad( const ResourceEvent& event )
{
	const TextHandle& handleText = HandleCast<Text>( event.handle );
	Text* text = handleText.Resolve();

	if( text->getResourceGroup() != ResourceGroup::Shaders )
		return;

	const ProgramPtr& program = createProgram(text);

	Path base = PathGetFileBase( text->getPath() );
	registerProgram( base, program );
}

//-----------------------------------//

void ProgramManager::onReload( const ResourceEvent& event )
{
	const TextHandle& handleText = HandleCast<Text>( event.handle );
	
	Text* text = handleText.Resolve();
	if( !text ) return;

	if( text->getResourceGroup() != ResourceGroup::Shaders )
		return;

	Path base = PathGetFileBase( text->getPath() );
	ProgramPtr program = programs[base];

	if(program)
	{
		LogDebug( "Reloading shader '%s'", text->getPath().c_str() );
		program->updateShadersText();
	}
}

//-----------------------------------//

} // end namespace