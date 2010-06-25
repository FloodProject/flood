/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/gui/Label.h"
#include "vapor/resources/Font.h"
#include "vapor/render/Material.h"
#include "vapor/scene/Geometry.h"
#include "vapor/resources/ResourceManager.h"
#include "vapor/Engine.h"

namespace vapor {

const std::string& Label::type = "Label";

//-----------------------------------//

Label::Label( const std::string& text, FontPtr font, MaterialPtr material )
	: text(text),
	font(font),
	material(material)
{
	init();
}

//-----------------------------------//

Label::Label( const std::string& text, const std::string& uri )
	: text(text)
{
	static int i = 0;

	material = new Material("FontMaterial" + String::fromNumber(i++), "Tex");
	
	ResourceManager* rm = Engine::getInstance().getResourceManager();
	font = rm->loadResource<Font>(uri);

	init();
}

//-----------------------------------//

void Label::init()
{
	isDirty = true;
	setupDone = false;

	VertexBufferPtr vb( new VertexBuffer() );
	renderable = new Renderable( Primitive::Quads, vb, material );

	// Add a new renderable to hold the text geometry
	addRenderable( renderable, RenderGroup::Overlays );
}

//-----------------------------------//

void Label::setText( const std::string& newText )
{
	text = newText;
	isDirty = true;
}

//-----------------------------------//

void Label::setupState()
{
	if( setupDone )
		return;

	assert( font );
	assert( font->isLoaded() );
	assert( font->getImage() );

	MaterialPtr material( renderable->getMaterial() );

	// Setup the material to have the texture font and enable blending
	material->setTexture( 0, font->getImage() );
	material->setBlending( BlendingSource::SourceAlpha, BlendingDestination::One );
	material->setProgram( "Tex" );

	setupDone = true;
}

//-----------------------------------//

void Label::update( double delta )
{
	if( !font )
		return;

	if( !font->isLoaded() )
		return;

	setupState();

	// No need to update geometry if the label did not change.
	if( isDirty )
		rebuildGeometry();

	Overlay::update( delta );
}

//-----------------------------------//

void Label::rebuildGeometry()
{
	const std::vector<Glyph>& glyphs = font->getGlyphs();

	const float width = font->getImage()->getWidth();
	const float height = font->getImage()->getHeight();

	const VertexBufferPtr& vb = renderable->getVertexBuffer();

	// Invalidate the existing vertex buffer contents
	vb->clear();

	// Calculate the quad vertices
	std::vector<Vector3> vertex;

	// Calculate the tex coords
	std::vector<Vector3> texcoords;

	ushort x_pos = 0;
	ushort y_pos = 0;

	ushort mid_offset = font->getGlyphSize().x /2;

	foreach( unsigned char c, text )
	{
		// If we find a line break, we start a new line.
		if( c == '\n' )
		{
			x_pos = 0;
			y_pos -= font->getGlyphSize().y;
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
	vb->set( VertexAttribute::MultiTexCoord0, texcoords );

	// No need to update geometry again until the text changes
	isDirty = false;
}

//-----------------------------------//

} // end namespace