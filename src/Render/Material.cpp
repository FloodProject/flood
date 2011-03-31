/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/Material.h"
#include "Render/Device.h"
#include "Render/TextureManager.h"
#include "Render/ProgramManager.h"
#include "Engine.h"
#include "Core/Utilities.h"

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

Material::~Material()
{
	LogDebug("Removing material '%s'", name.c_str());
}

//-----------------------------------//

void Material::init()
{
	alphaTest = false;
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
	RenderDevice* renderDevice = GetEngine()->getRenderDevice();
	TextureManager* tm = renderDevice->getTextureManager();
	
	TexturePtr tex = tm->getTexture( name );
	textures[unit] = tex;
}

//-----------------------------------//

void Material::setTexture( uint unit, const ImagePtr& img )
{
	Engine* engine = GetEngine();
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

TexturePtr Material::getTexture( uint unit )
{
	TextureMap::iterator it = textures.find(unit);
	
	if( it == textures.end() )
		return nullptr;

	return textures[unit];
}

//-----------------------------------//

bool Material::isBlendingEnabled() const
{
	return _isBlendingEnabled;
}

//-----------------------------------//

void Material::setBlending( BlendSource::Enum source, BlendDestination::Enum destination )
{
	this->source = source;
	this->destination = destination;
	
	_isBlendingEnabled = true;
}

//-----------------------------------//

ProgramPtr Material::getProgram()
{
	RenderDevice* renderDevice = GetEngine()->getRenderDevice();
	ProgramManager* programManager = renderDevice->getProgramManager();

	return programManager->getProgram( program );
}

//-----------------------------------//

void Material::bindTextures(bool bindUniforms)
{
	TextureMap::const_iterator it;
	for( it = textures.begin(); it != textures.end(); it++ )
	{
		int index = it->first;
		const TexturePtr& tex = it->second;

		tex->bind(index);

		if(!bindUniforms) continue;

		const ProgramPtr& program = getProgram();
		
		if( !program ) continue;
		
		std::string uniform = "vp_Texture" + StringFromNumber(index);
		program->setUniform( uniform, index );
	}
}

//-----------------------------------//

void Material::unbindTextures()
{
	TextureMap::const_iterator it;
	for( it = textures.begin(); it != textures.end(); it++ )
	{
		const TexturePtr& tex = it->second;
		tex->unbind( it->first );
	}
}

//-----------------------------------//

} // end namespace