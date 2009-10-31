/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/gui/Overlay.h"

namespace vapor {
	namespace gui {

//-----------------------------------//

const std::string& Overlay::type = "Overlay";

//-----------------------------------//

Overlay::Overlay() 
	: anchor( Anchor::TopLeft ), positioning( Positioning::Absolute ),
	x( 0 ), y( 0 )
{

}

//-----------------------------------//

Overlay::~Overlay()
{

}

//-----------------------------------//

Positioning::Enum Overlay::getPositioning() const
{
	return positioning;
}

//-----------------------------------//

void Overlay::setPositioning( Positioning::Enum positioning )
{
	this->positioning = positioning;
}

//-----------------------------------//

Anchor::Enum Overlay::getAnchor() const
{
	return anchor;
}

//-----------------------------------//

void Overlay::setAnchor( Anchor::Enum anchor )
{
	this->anchor = anchor;
}

//-----------------------------------//

//std::pair<int,int> Overlay::getPosition() const
//{
//	return std::make_pair(x, y);
//}

//-----------------------------------//

//void Overlay::setPosition( int x, int y )
//{
//	this->x = x;
//	this->y = y;
//}

//-----------------------------------//

void Overlay::update( float UNUSED(delta) )
{

}

//-----------------------------------//

const std::string Overlay::save( int UNUSED(indent) )
{
	return ""; 
}

//-----------------------------------//

const std::string& Overlay::getType() const 
{
	return type; 
}

//-----------------------------------//

} } // end namespaces