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

Renderable::Renderable(PrimitiveType::Enum primitive, 
						tr1::shared_ptr<VertexBuffer> vb, 
						tr1::shared_ptr<IndexBuffer> ib, 
						tr1::shared_ptr<Material>)
{

}

//-----------------------------------//

Renderable::Renderable(PrimitiveType::Enum primitive, 
						tr1::shared_ptr<VertexBuffer> vb, 
						tr1::shared_ptr<Material>)
{

}

//-----------------------------------//

Renderable::Renderable(PrimitiveType::Enum primitive, 
						tr1::shared_ptr<VertexBuffer> vb)
{

}

//-----------------------------------//

void Renderable::render(render::Device &device) const
{

}

//-----------------------------------//

tr1::shared_ptr<VertexBuffer> Renderable::getVertexBuffer() const
{
	return vertexBuffer;
}

//-----------------------------------//

tr1::shared_ptr<IndexBuffer> Renderable::getIndexBuffer() const
{
	return indexBuffer;
}

//-----------------------------------//

tr1::shared_ptr<Material> Renderable::getMaterial() const
{
	return material;
}

//-----------------------------------//

} } // end namespaces