/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Renderable.h"

namespace vapor {

//-----------------------------------//

Renderable::Renderable() 
	: mode( PolygonMode::Solid )
{ }

//-----------------------------------//

Renderable::Renderable( PolygonType::Enum type,
						const VertexBufferPtr& vb, 
						const IndexBufferPtr& ib, 
						const MaterialPtr& mat )
	: type( type )
	, vb( vb )
	, ib( ib )
	, mat( mat )
	, mode( PolygonMode::Solid )
{ }

//-----------------------------------//

Renderable::Renderable( PolygonType::Enum type, 
						const VertexBufferPtr& vb, 
						const MaterialPtr& mat )
	: type( type )
	, vb( vb )
	, mat( mat )
	, mode( PolygonMode::Solid )
{ }

//-----------------------------------//

bool Renderable::bind()
{
	if( !mat || !vb )
		return false;

	mat->bind();

	if( !vb->isBuilt() )
		vb->build();

	vb->bind();

	if( !ib )
		return true;

	if( !ib->isBuilt() )
		ib->build();
	
	ib->bind();

	return true;
}

//-----------------------------------//

bool Renderable::unbind()
{
	if( !mat || !vb )
		return false;

	if( ib )
		ib->unbind();
    
	vb->unbind();
	mat->unbind();

	return true;
}

//-----------------------------------//

void Renderable::render( RenderDevice* )
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
        GLenum size = ib->is16bit() ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
        glDrawElements( type, ib->getNumIndices(), size, 0 );

		if( glHasError("Error drawing index buffer") )
			return;
    }
   
	if( mode == PolygonMode::Wireframe )
		glPolygonMode( GL_FRONT_AND_BACK, PolygonMode::Solid );
}

//-----------------------------------//

} // end namespace