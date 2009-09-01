/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/Renderable.h"

namespace vapor {
	namespace render {

//-----------------------------------//

Renderable::Renderable( PrimitiveType::Enum primitive, 
						tr1::shared_ptr< VertexBuffer > vb, 
						tr1::shared_ptr< IndexBuffer > ib, 
						tr1::shared_ptr< Material > mat )
	: type( primitive), vb( vb ), ib( ib ), mat( mat )
{

}

//-----------------------------------//

Renderable::Renderable( PrimitiveType::Enum primitive, 
						tr1::shared_ptr< VertexBuffer > vb, 
						tr1::shared_ptr< Material > )
	: type( primitive), vb( vb ), mat( mat )

{

}

//-----------------------------------//

void Renderable::render( render::Device& device ) const
{

}

//-----------------------------------//

tr1::shared_ptr<VertexBuffer> Renderable::getVertexBuffer() const
{
	return vb;
}

//-----------------------------------//

tr1::shared_ptr<IndexBuffer> Renderable::getIndexBuffer() const
{
	return ib;
}

//-----------------------------------//

tr1::shared_ptr<Material> Renderable::getMaterial() const
{
	return mat;
}

//-----------------------------------//

} } // end namespaces