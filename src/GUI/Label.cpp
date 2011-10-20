/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "GUI/Label.h"
#include "Core/Utilities.h"
#include "Resources/ResourceManager.h"
#include "Scene/Geometry.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Label, Overlay)
	FIELD_PRIMITIVE_SETTER(4, string, text, Text)
	FIELD_PRIMITIVE(5, bool, isDirty) FIELD_READONLY(isDirty) FIELD_NO_SERIALIZE(isDirty)
	FIELD_CLASS_PTR_SETTER(6, Font, FontHandle, font, Handle, Font) 
REFLECT_CLASS_END()

//-----------------------------------//

Label::Label()
{
	init();
}

//-----------------------------------//

void Label::init()
{
	String name = StringFormat("FontMaterial");
	
	// Setup the material.
	Material* pMaterial = material.Resolve();
	pMaterial->setDepthTest(false);
	pMaterial->setBackfaceCulling(false);
	pMaterial->setBlending( BlendSource::SourceAlpha, BlendDestination::One );
	pMaterial->setProgram( "Tex" );

	isDirty = true;
	setupDone = false;
}

//-----------------------------------//

void Label::setText( const String& newText )
{
	text = newText;
	isDirty = true;
}

//-----------------------------------//

void Label::setFont( const FontHandle& newFont )
{
	font = newFont;

	Font* pFont = font.Resolve();

	if( pFont )
	{
		MaterialPtr pMaterial = material.Resolve();
		pMaterial->setTexture( 0, pFont->getImage() );
	}

	isDirty = true;
}

//-----------------------------------//

void Label::update( float delta )
{
	// No need to update geometry if the label did not change.
	//if( !isDirty ) return;

	Overlay::update( delta );
}

//-----------------------------------//

void Label::rebuildGeometry()
{
	const GeometryBufferPtr& gb = renderable->getGeometryBuffer();
	
	// Invalidate the existing vertex buffer contents
	gb->declarations.reset();
	gb->clear();

	// Calculate the quad vertices
	std::vector<Vector3> vertex;

	// Calculate the tex coords
	std::vector<Vector3> texcoords;

	uint16 x_pos = 0;
	uint16 y_pos = 0;

	Font* pFont = font.Resolve();
	if( !pFont ) return;

	const std::vector<Glyph>& glyphs = pFont->getGlyphs();

	Image* image = pFont->getImage().Resolve();
	if( !image ) return;

	const float width = image->getWidth();
	const float height = image->getHeight();

	uint16 mid_offset = pFont->getGlyphSize().x /2;

	for( size_t i = 0; i < text.size(); i++ )
	{
		unsigned char c = text[i];

		// If we find a line break, we start a new line.
		if( c == '\n' )
		{
			x_pos = 0;
			y_pos -= pFont->getGlyphSize().y;
		}

		if( c >= glyphs.size() ) continue;

		// We need each glyph information to calculate positions and size.
		const Glyph& glyph = glyphs[c];

		vertex.push_back( Vector2( x_pos, y_pos ) );
		vertex.push_back( Vector2( x_pos, y_pos + glyph.height ) );
		vertex.push_back( Vector2( x_pos + glyph.width, y_pos + glyph.height ) );
		vertex.push_back( Vector2( x_pos + glyph.width, y_pos ) );
	
		float glyph_x_left = ::ceilf(glyph.x + mid_offset - glyph.width / 2);
		float glyph_x_right = ::ceilf(glyph.x + mid_offset + glyph.width / 2);

		texcoords.push_back( Vector2( glyph_x_left / width, glyph.y / height ) );
		texcoords.push_back( Vector2( glyph_x_left / width, (glyph.y + glyph.height) / height ) );
		texcoords.push_back( Vector2( glyph_x_right / width, (glyph.y + glyph.height) / height ) );
		texcoords.push_back( Vector2( glyph_x_right / width, glyph.y / height ) );

		x_pos += glyph.width + 1;
	}

	gb->set( VertexAttribute::Position, vertex );
	gb->set( VertexAttribute::TexCoord0, texcoords );

	// No need to update geometry again until the text changes
	isDirty = false;

	updateBounds();
}

//-----------------------------------//

NAMESPACE_ENGINE_END