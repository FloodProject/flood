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
#include "vapor/Utilities.h"

namespace vapor { namespace render {

using namespace vapor::resources;

float Material::DefaultLineWidth = 1.0f;

//-----------------------------------//

Material::Material( const std::string& name, ProgramPtr program )
	: name( name ), program( program )
{
	init();
}

//-----------------------------------//

Material::Material( const std::string& name, const std::string& program )
	: name( name ),
	program( ProgramManager::getInstance().getProgram(program) )
{
	init();
}

//-----------------------------------//

Material::~Material()
{

}

//-----------------------------------//

void Material::init()
{
	_isBlendingEnabled = false;
	src = BlendingSource::One;
	dst = BlendingDestination::Zero;
	
	lineWidth = DefaultLineWidth;
	lineSmooth = false;

	depthTest = true;
	depthWrite = true;

	cullBackfaces = true;
}

//-----------------------------------//

void Material::setTexture( uint unit, const std::string& name )
{
	TexturePtr tex = TextureManager::getInstance().getTexture( name );
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

void Material::setBlending( BlendingSource::Enum src, 
	BlendingDestination::Enum dst )
{
	this->src = src;
	this->dst = dst;
	
	_isBlendingEnabled = true;
}

//-----------------------------------//

void Material::setProgram( const std::string& name )
{
	ProgramPtr p = ProgramManager::getInstance().getProgram( name );
	program = p;
}

//-----------------------------------//

void Material::bind()
{
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
	program->unbind();

	foreach( const TextureMapPair& p, getTextures() )
	{
		p.second->unbind( p.first );
	}
}

//-----------------------------------//

} } // end namespaces