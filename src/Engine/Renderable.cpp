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

Renderable::Renderable( Primitive::Enum primitive,
						const VertexBufferPtr& vb, 
						const IndexBufferPtr& ib, 
						const MaterialPtr& mat )
	: type( primitive), 
	vb( vb ), ib( ib ), mat( mat ),
	mode( PolygonMode::Solid )
{ }

//-----------------------------------//

Renderable::Renderable( Primitive::Enum primitive, 
						const VertexBufferPtr& vb, 
						const MaterialPtr& mat )
	: type( primitive),
	vb( vb ), mat( mat ),
	mode( PolygonMode::Solid )
{ }

//-----------------------------------//

void Renderable::bind()
{
	if( !mat || !vb )
		return;

	mat->bind();

	if( !vb->isBuilt() )
		vb->build();

	vb->bind();

	if( ib )
	{
		if( !ib->isBuilt() )
			ib->build();

        ib->bind();
	}
}

//-----------------------------------//

void Renderable::unbind()
{
	if( !mat || !vb )
		return;

	if( ib )
		ib->unbind();
    
	vb->unbind();
	mat->unbind();
}

//-----------------------------------//

void Renderable::render( const RenderDevicePtr& VAPOR_UNUSED(device) )
{
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
        GLenum gltype = ib->is16bit() ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
        glDrawElements( type, ib->getNumIndices(), gltype, 0 );

		if( glHasError("Error drawing index buffer") )
			return;
    }
   
	if( mode == PolygonMode::Wireframe )
		glPolygonMode( GL_FRONT_AND_BACK, PolygonMode::Solid );
}

//-----------------------------------//

} // end namespace