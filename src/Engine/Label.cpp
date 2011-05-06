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

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(Label, Overlay)
	FIELD_PRIMITIVE(string, text)
	FIELD_PRIMITIVE(bool, isDirty)
	FIELD_CLASS_PTR(Font, FontHandle, font, Handle) 
REFLECT_CLASS_END()

//-----------------------------------//

Label::Label()
{ }

//-----------------------------------//

Label::Label( const String& text, const FontHandle& font, const MaterialHandle& material )
	: text(text)
	, font(font)
{
	init();
	renderable->setMaterial(material);
}

//-----------------------------------//

Label::Label( const String& text, const String& path )
	: text(text)
{
	static int i = 0;

	// material = new Material("FontMaterial" + StringFromNumber(i++), "Tex");
	
	ResourceManager* res = GetResourceManager();
	font = res->loadResource<Font>(path);

	init();
}

//-----------------------------------//

void Label::init()
{
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

void Label::setupState()
{
	if( setupDone )
		return;

	Font* pFont = font.Resolve();

	assert( pFont->isLoaded() );
	assert( pFont->getImage() );

	MaterialHandle handleMaterial = renderable->getMaterial();
	Material* material = handleMaterial.Resolve();

	// Setup the material to have the texture font and enable blending
	material->setTexture( 0, pFont->getImage() );
	material->setBlending( BlendSource::SourceAlpha, BlendDestination::One );
	material->setProgram( "Tex" );

	setupDone = true;
}

//-----------------------------------//

void Label::update( float delta )
{
	Font* pFont = font.Resolve();

	if( !pFont ) return;
	if( !pFont->isLoaded() ) return;

	setupState();

	// No need to update geometry if the label did not change.
	if( isDirty )
		rebuildGeometry();

	//Overlay::update( delta );
}

//-----------------------------------//

void Label::rebuildGeometry()
{
	const VertexBufferPtr& vb = renderable->getVertexBuffer();

	// Invalidate the existing vertex buffer contents
	vb->clear();

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

		// We need each glyph information to calculate positions and size.
		const Glyph& glyph = glyphs[c];

		vertex.push_back( Vector2( x_pos, y_pos ) );
		vertex.push_back( Vector2( x_pos, y_pos - glyph.height ) );
		vertex.push_back( Vector2( x_pos + glyph.width, y_pos - glyph.height ) );
		vertex.push_back( Vector2( x_pos + glyph.width, y_pos ) );
	
		float glyph_x_left = ::ceilf(glyph.x + mid_offset - glyph.width / 2);
		float glyph_x_right = ::ceilf(glyph.x + mid_offset + glyph.width / 2);

		texcoords.push_back( Vector2( glyph_x_left / width, glyph.y / height ) );
		texcoords.push_back( Vector2( glyph_x_left / width, (glyph.y + glyph.height) / height ) );
		texcoords.push_back( Vector2( glyph_x_right / width, (glyph.y + glyph.height) / height ) );
		texcoords.push_back( Vector2( glyph_x_right / width, glyph.y / height ) );

		x_pos += glyph.width + 1;
	}

	vb->set( VertexAttribute::Position, vertex );
	vb->set( VertexAttribute::TexCoord0, texcoords );

	// No need to update geometry again until the text changes
	isDirty = false;
}

//-----------------------------------//

} // end namespace