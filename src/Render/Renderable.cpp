/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/Renderable.h"
#include "Render/Device.h"
#include "Render/GL.h"

NAMESPACE_BEGIN

//-----------------------------------//

Renderable* RenderableCreate( Allocator* alloc )
{
	Renderable* rend = Allocate(Renderable, alloc);
	return rend;
}

//-----------------------------------//

Renderable::Renderable()
	: type( PolygonType::Triangles )
	, mode( PolygonMode::Solid )
	, stage( RenderLayer::Normal )
	, priority(0)
{
	ub = Allocate(UniformBuffer, GetRenderAllocator());
}

//-----------------------------------//

bool Renderable::bind()
{
	if( !vb ) return false;

	if( !vb->isBuilt() )
		vb->build();

	vb->bind();

	if( !ib ) return true;

	ib->bind();
	
	if( !ib->isBuilt ) ib->build();

	return true;
}

//-----------------------------------//

bool Renderable::unbind()
{
	if( !vb )
		return false;

	if( ib )
		ib->unbind();
    
	vb->unbind();

	return true;
}

//-----------------------------------//

void Renderable::render( RenderDevice* device )
{
	#pragma TODO("Move polygon modes to the renderer")

	if( mode == PolygonMode::Wireframe )
		glPolygonMode( GL_FRONT_AND_BACK, PolygonMode::Wireframe );

    if( !ib )
    {
        glDrawArrays( type, 0, vb->getNumVertices() );

 		if( glHasError("Error drawing vertex buffer") )
			return;
    }
    else
    {
        GLenum size = (ib->indexSize == 16) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
        glDrawElements( type, ib->getSize(), size, 0 );

		if( glHasError("Error drawing index buffer") )
			return;
    }
   
	if( mode == PolygonMode::Wireframe )
		glPolygonMode( GL_FRONT_AND_BACK, PolygonMode::Solid );
}

//-----------------------------------//

NAMESPACE_END