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
using namespace vapor::render	;

namespace vapor {
	namespace gui {

//-----------------------------------//

Label::Label( std::string text, resources::FontPtr font, 
			 render::MaterialPtr mat, Anchor::Enum anchor )
	: font( font ), text( text), anchor( anchor ), x( 0 ), y( 0 ), 
	material( mat ), isDirty( true )
{

}

//-----------------------------------//

Label::Label( std::string text, std::string name, Anchor::Enum anchor )
	: text( text), anchor( anchor ), x( 0 ), y( 0 ),
	font( ResourceManager::getInstance().loadResource<Font>(name) )
{

}

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
	if( !isDirty ) return;

	// Create a new quad with the text characters
	QuadPtr quad( new  Quad() );
	
	material->setTexture( 0, font->getTexture() );
	quad->setMaterial( material );

	addRenderable( quad );

	isDirty = false;
}

//-----------------------------------//

} } // end namespaces