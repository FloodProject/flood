/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Geometry.h"

using namespace vapor::render;

namespace vapor {
	namespace scene {

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

void Geometry::update( double delta )
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