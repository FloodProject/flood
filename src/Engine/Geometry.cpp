/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Geometry.h"

namespace vapor {
	namespace scene {

using namespace vapor::render;

//-----------------------------------//

Geometry::Geometry()
{

}

//-----------------------------------//

Geometry::Geometry( RenderablePtr rend )
{
	renderables.push_back( rend );
}

//-----------------------------------//

Geometry::~Geometry()
{

}

//-----------------------------------//

void Geometry::addRenderable( RenderablePtr rend )
{
	renderables.push_back( rend );
}

//-----------------------------------//

void Geometry::update( )
{

}

//-----------------------------------//

const std::string Geometry::save(int indent)
{
	return "";
}

//-----------------------------------//

const std::string Geometry::name() const
{
	return "Geometry";
}

//-----------------------------------//

} } // end namespaces