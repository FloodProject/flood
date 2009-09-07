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

Renderable::Renderable( Primitive::Enum primitive,
						VertexBufferPtr vb, 
						IndexBufferPtr ib, 
						MaterialPtr mat )
	: type( primitive), vb( vb ), ib( ib ), mat( mat )
{

}

//-----------------------------------//

Renderable::Renderable( Primitive::Enum primitive, 
						VertexBufferPtr vb, 
						MaterialPtr mat )
	: type( primitive), vb( vb ), mat( mat )

{

}

//-----------------------------------//

void Renderable::render( render::Device& device )
{
    vb->bind();
    
    if ( ib == nullptr )
    {
        glDrawArrays( type, 0, vb->getSize() );

		// check for errors
		if( glGetError() != GL_NO_ERROR )
		{
			warn( "gl::buffers", "Error drawing renderable" );
			debug( "shit happened, debug me" );
		}
    }
    //else
    //{
    //    ib->bind();
    //    
    //    GLenum type = ib->is16() ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
    //    glDrawElements( primitiveType, ib->getNumIndices(), type, 0 );

    //    ib->unbind();
    //}
    
    vb->unbind();
}

//-----------------------------------//

VertexBufferPtr Renderable::getVertexBuffer() const
{
	return vb;
}

//-----------------------------------//

IndexBufferPtr Renderable::getIndexBuffer() const
{
	return ib;
}

//-----------------------------------//

MaterialPtr Renderable::getMaterial() const
{
	return mat;
}

//-----------------------------------//

void Renderable::setMaterial( MaterialPtr mat )
{
	this->mat = mat;
}

//-----------------------------------//

} } // end namespaces