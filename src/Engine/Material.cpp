/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Material.h"
#include "vapor/render/ProgramManager.h"
#include "vapor/render/Device.h"
#include "vapor/Engine.h"

namespace vapor { namespace render {

using namespace vapor::resources;

float Material::DefaultLineWidth = 1.0f;

//-----------------------------------//

Material::Material( const std::string& _name, const std::string& _program )
	: name( _name ),
	program( _program )
{
	init();
}

//-----------------------------------//

void Material::init()
{
	_isBlendingEnabled = false;
	source = BlendingSource::One;
	destination = BlendingDestination::Zero;
	
	lineWidth = DefaultLineWidth;
	lineSmooth = false;

	depthTest = true;
	depthWrite = true;

	cullBackfaces = true;
}

//-----------------------------------//

void Material::setTexture( uint unit, const std::string& name )
{
	// TODO: refactor
	TextureManagerPtr tm = Engine::getInstance().getRenderDevice()->getTextureManager();
	TexturePtr tex = tm->getTexture( name );
	textures[unit] = tex;
}

//-----------------------------------//

void Material::setTexture( uint unit, const ImagePtr& img )
{
	// TODO: refactor
	TextureManagerPtr tm = Engine::getInstance().getRenderDevice()->getTextureManager();
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

void Material::setBlending( BlendingSource::Enum _source, 
	BlendingDestination::Enum _destination )
{
	source = _source;
	destination = _destination;
	
	_isBlendingEnabled = true;
}

//-----------------------------------//

ProgramPtr Material::getProgram()
{
	render::DevicePtr device = Engine::getInstance().getRenderDevice();
	ProgramManagerPtr pm = device->getProgramManager();

	return pm->getProgram( program );
}

//-----------------------------------//

void Material::bind()
{
	ProgramPtr program = getProgram();

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
		program->setUniform( "tex" + num_to_str(p.first), p.first );
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

} } // end namespaces