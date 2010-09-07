/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Material.h"
#include "vapor/render/Device.h"
#include "vapor/render/TextureManager.h"
#include "vapor/render/ProgramManager.h"
#include "vapor/Engine.h"

namespace vapor {

// Default line width.
float Material::DefaultLineWidth = 1.0f;

//-----------------------------------//

Material::Material( const std::string& name, const std::string& program )
	: name(name)
	, program(program)
{
	init();
}

//-----------------------------------//

Material::Material( const Material& rhs )
	: name(rhs.name)
	, program(rhs.program)
	, textures(rhs.textures)
	, cullBackfaces(rhs.cullBackfaces)
	, depthCompare(rhs.depthCompare)
	, depthTest(rhs.depthTest)
	, depthWrite(rhs.depthWrite)
	, lineSmooth(rhs.lineSmooth)
	, lineWidth(rhs.lineWidth)
	, source(rhs.source)
	, destination(rhs.destination)
	, _isBlendingEnabled(rhs._isBlendingEnabled)
{
	init();
}

//-----------------------------------//

void Material::init()
{
	depthCompare = DepthCompare::Less;
	depthTest = true;
	depthWrite = true;
	lineSmooth = false;
	cullBackfaces = true;
	lineWidth = DefaultLineWidth;
	_isBlendingEnabled = false;
	source = BlendSource::One;
	destination = BlendDestination::Zero;
}

//-----------------------------------//

void Material::setTexture( uint unit, const std::string& name )
{
	Engine* engine = Engine::getInstancePtr();
	RenderDevice* renderDevice = engine->getRenderDevice();
	TextureManager* tm = renderDevice->getTextureManager();
	
	TexturePtr tex = tm->getTexture( name );
	textures[unit] = tex;
}

//-----------------------------------//

void Material::setTexture( uint unit, const ImagePtr& img )
{
	Engine* engine = Engine::getInstancePtr();
	RenderDevice* renderDevice = engine->getRenderDevice();
	TextureManager* tm = renderDevice->getTextureManager();

	TexturePtr tex = tm->getTexture( img );
	textures[unit] = tex;
}

//-----------------------------------//

void Material::setTexture( uint unit, const TexturePtr& tex )
{
	textures[unit] = tex;
}

//-----------------------------------//

bool Material::isBlendingEnabled() const
{
	return _isBlendingEnabled;
}

//-----------------------------------//

void Material::setBlending( BlendSource::Enum _source, 
	BlendDestination::Enum _destination )
{
	source = _source;
	destination = _destination;
	
	_isBlendingEnabled = true;
}

//-----------------------------------//

ProgramPtr Material::getProgram()
{
	Engine* engine = Engine::getInstancePtr();
	RenderDevice* renderDevice = engine->getRenderDevice();
	ProgramManager* programManager = renderDevice->getProgramManager();

	return programManager->getProgram( program );
}

//-----------------------------------//

void Material::bind()
{
	ProgramPtr program = getProgram();
	assert( program != nullptr );

	foreach( const TextureMapPair& p, textures )
	{
		p.second->bind( p.first );
	}

	if( !program->isLinked() )
	{
		program->link();
	}

	program->bind();

	foreach( const TextureMapPair& p, textures )
	{
		std::string name = "vp_Texture"+String::fromNumber(p.first);
		program->setUniform( name, (int)p.first );
	}
}

//-----------------------------------//

void Material::unbind()
{
	ProgramPtr program = getProgram();

	program->unbind();

	foreach( const TextureMapPair& p, getTextures() )
	{
		p.second->unbind( p.first );
	}
}

//-----------------------------------//

} // end namespace