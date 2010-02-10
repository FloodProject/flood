/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/gui/Label.h"

#include "vapor/resources/ResourceManager.h"

using namespace vapor::resources;
using namespace vapor::render;
using namespace vapor::math;

namespace vapor { namespace gui {

//-----------------------------------//

const std::string& Label::type = "Label";

//-----------------------------------//

Label::Label( const std::string& text, resources::FontPtr font, 
			 render::MaterialPtr material )
	: font( font ), text( text ), isDirty( true ), 
	renderable( new Renderable( Primitive::Quads, VertexBufferPtr( new VertexBuffer() ), material ) )
{
	// Add a new renderable to hold the text geometry
	addRenderable( renderable, RenderGroup::Overlays );
	
	// Setup the material to have the texture font and enable blending
	material->setTexture( 0, font->getTexture() );
	material->setBlending( BlendingOperationSource::SourceAlpha, BlendingOperationDestination::One );
}

//-----------------------------------//

//Label::Label( std::string text, std::string name, Anchor::Enum anchor )
//	: text( text ), anchor( anchor ), x( 0 ), y( 0 ),
//	font( ResourceManager::getInstance().loadResource<Font>(name) )
//{
//
//}

//-----------------------------------//

Label::~Label()
{

}

//-----------------------------------//

const std::string& Label::getText() const
{
	return text;
}

//-----------------------------------//

void Label::setText( const std::string& text )
{
	this->text = text;
	isDirty = true;
}

//-----------------------------------//

void Label::update( double UNUSED(delta) )
{
	// No need to update geometry if the label did not change.
	if( !isDirty || text.empty() ) return;

	std::vector<Glyph> glyphs = font->getGlyphsTable();

	const float width = font->getTexture()->getImage()->getWidth();
	const float height = font->getTexture()->getImage()->getHeight();

	VertexBufferPtr vb = renderable->getVertexBuffer();

	// Invalidate the existing vertex buffer contents
	vb->clear();

	// Calculate the quad vertices
	std::vector<Vector3> vertex;

	// Calculate the tex coords
	std::vector<Vector3> texcoords;

	ushort x_pos = 0; ushort y_pos = 0;
	ushort mid_offset = font->getGlyphSize().first/2;

	foreach( unsigned char c, text )
	{
		// If we find a line break, we start a new line.
		if( c == '\n' )
		{
			x_pos = 0;
			y_pos -= font->getGlyphSize().second;
		}

		// We need each glyph information to calculate positions and size.
		const Glyph& glyph = glyphs[c];

		vertex.push_back( Vector3( x_pos, y_pos, 0.0f ) );
		vertex.push_back( Vector3( x_pos, y_pos - (float)glyph.height, 0.0f ) );
		vertex.push_back( Vector3( x_pos + (float)glyph.width, y_pos - (float)glyph.height, 0.0f ) );
		vertex.push_back( Vector3( x_pos + (float)glyph.width, y_pos, 0.0f ) );
	
		float glyph_x_left = ::ceilf(glyph.x + mid_offset - (float)glyph.width / 2);
		float glyph_x_right = ::ceilf(glyph.x + mid_offset + (float)glyph.width / 2);

		texcoords.push_back( Vector3( glyph_x_left / width, glyph.y / height, 0.0f ) );
		texcoords.push_back( Vector3( glyph_x_left / width, (glyph.y + (float)glyph.height) / height, 0.0f ) );
		texcoords.push_back( Vector3( glyph_x_right / width, (glyph.y + (float)glyph.height) / height, 0.0f ) );
		texcoords.push_back( Vector3( glyph_x_right / width, glyph.y / height, 0.0f ) );

		x_pos += glyph.width + 1;
	}

	vb->set( VertexAttribute::Position, vertex );
	vb->set( VertexAttribute::MultiTexCoord0, texcoords );

	// No need to update geometry again until the text changes
	isDirty = false;
}

//-----------------------------------//

const std::string& Label::getType() const 
{
	return type; 
}

//-----------------------------------//

const std::string Label::save( int indent )
{
	return ""; 
}

//-----------------------------------//

} } // end namespaces