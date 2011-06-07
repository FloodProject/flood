/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/Material.h"
#include "Resources/ResourceManager.h"
#include "Core/Log.h"

NAMESPACE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Material, Resource)
REFLECT_CLASS_END()

//-----------------------------------//

MaterialHandle MaterialCreate( Allocator* alloc, const String& name )
{
	Material* material = Allocate(Material, alloc);
	material->setName(name);

	return HandleCast<Material>( ResourceHandleCreate(material) );
}

float Material::DefaultLineWidth = 1.0f;

//-----------------------------------//

Material::Material()
{
	init();
	program = "VertexColor";
}

//-----------------------------------//

Material::Material( const String& name, const String& program )
	: name(name)
	, program(program)
{
	init();
}

//-----------------------------------//

Material::Material( const Material& rhs )
	: name(rhs.name)
	, program(rhs.program)
	, cullBackfaces(rhs.cullBackfaces)
	, depthCompare(rhs.depthCompare)
	, depthTest(rhs.depthTest)
	, depthWrite(rhs.depthWrite)
	, lineSmooth(rhs.lineSmooth)
	, lineWidth(rhs.lineWidth)
	, source(rhs.source)
	, destination(rhs.destination)
	, _isBlendingEnabled(rhs._isBlendingEnabled)
	, textureUnits(rhs.textureUnits)
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

void Material::setTexture( uint32 unit, const String& name )
{
	ResourceManager* res = GetResourceManager();
	
	ImageHandle handle = res->loadResource<Image>(name);
	if( !handle ) return;

	textureUnits[unit] = handle;
}

//-----------------------------------//

void Material::setTexture( uint32 unit, const ImageHandle& handle )
{
	textureUnits[unit] = handle;
}

//-----------------------------------//

ImageHandle Material::getTexture( uint32 unit )
{
	return textureUnits[unit];
}

//-----------------------------------//

NAMESPACE_END