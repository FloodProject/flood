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

Renderable::Renderable()
{

}

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

Renderable::~Renderable()
{

}

//-----------------------------------//

void Renderable::render( render::Device& UNUSED(device) )
{
	if( mat ) mat->bind();

	if( !vb->isBuilt() )
		vb->build();

	vb->bind();

    if ( ib == nullptr )
    {
        glDrawArrays( type, 0, vb->getNumVertices() );

#ifdef VAPOR_DEBUG
		// check for errors
		while( glGetError() != GL_NO_ERROR )
		{
			warn( "gl::buffers", "Error drawing renderable" );
		}
#endif
    }
    else
    {
		if( !ib->isBuilt() )
			ib->build();

        ib->bind();
        
        GLenum gltype = ib->is16bit() ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
        glDrawElements( type, ib->getNumIndices(), gltype, 0 );

        ib->unbind();
    }
    
    vb->unbind();
	if( mat ) mat->unbind();
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

void Renderable::setType( Primitive::Enum p )
{
	this->type = p;
}

//-----------------------------------//

void Renderable::setVertexBuffer( VertexBufferPtr vb )
{
	this->vb = vb;
}

//-----------------------------------//

void Renderable::setIndexBuffer( IndexBufferPtr ib )
{
	this->ib = ib;
}

//-----------------------------------//

} } // end namespaces