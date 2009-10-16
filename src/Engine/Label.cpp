/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/gui/Label.h"

#include "vapor/resources/ResourceManager.h"
#include "vapor/render/Quad.h"

using namespace vapor::resources;
using namespace vapor::render;
using namespace vapor::math;

namespace vapor {
	namespace gui {

//-----------------------------------//

Label::Label( const std::string& text, resources::FontPtr font, 
			 render::MaterialPtr material, Anchor::Enum anchor )
	: font( font ), text( text ), anchor( anchor ), x( 0 ), y( 0 ), isDirty( true ), 
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

void Label::setText( std::string text )
{
	isDirty = true;
	this->text = text;
}

//-----------------------------------//

Anchor::Enum Label::getAnchor() const
{
	return anchor;
}

//-----------------------------------//

void Label::setAnchor( Anchor::Enum anchor )
{
	this->anchor = anchor;
}

//-----------------------------------//

std::pair<int,int> Label::getPosition() const
{
	return std::make_pair(x, y);
}

//-----------------------------------//

void Label::setPosition( int x, int y )
{
	this->x = x;
	this->y = y;
}

//-----------------------------------//

void Label::update( double delta )
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

	int x_pos = 0;
	int mid_offset = font->getGlyphSize().first/2;

	foreach( unsigned char c, text )
	{
		// We need each glyph information to calculate positions and size
		const Glyph& glyph = glyphs[c];

		vertex.push_back( Vector3( x_pos + 0.0f , 0.0f, 0.0f ) );
		vertex.push_back( Vector3( x_pos + 0.0f, -glyph.height, 0.0f ) );
		vertex.push_back( Vector3( x_pos + glyph.width, -glyph.height, 0.0f ) );
		vertex.push_back( Vector3( x_pos + glyph.width, 0.0f, 0.0f ) );
	
		int glyph_x_left = ::ceilf(glyph.x + mid_offset - glyph.width / 2);
		int glyph_x_right = ::ceilf(glyph.x + mid_offset + glyph.width / 2);

		texcoords.push_back( Vector3( glyph_x_left / width, glyph.y / height, 0.0f ) );
		texcoords.push_back( Vector3( glyph_x_left / width, (glyph.y + glyph.height) / height, 0.0f ) );
		texcoords.push_back( Vector3( glyph_x_right / width, (glyph.y + glyph.height) / height, 0.0f ) );
		texcoords.push_back( Vector3( glyph_x_right / width, glyph.y / height, 0.0f ) );

		x_pos += glyph.width + 1;
	}

	vb->set( VertexAttribute::Vertex, vertex );
	vb->set( VertexAttribute::MultiTexCoord0, texcoords );

	// No need to update geometry again until the text changes
	isDirty = false;
}

//-----------------------------------//

} } // end namespaces