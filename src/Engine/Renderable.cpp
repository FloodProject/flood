/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Renderable.h"

namespace vapor { namespace render {

//-----------------------------------//

Renderable::Renderable() 
	: mode( PolygonMode::Solid )
{

}

//-----------------------------------//

Renderable::Renderable( Primitive::Enum primitive,
						const VertexBufferPtr& vb, 
						const IndexBufferPtr& ib, 
						const MaterialPtr& mat )
	: type( primitive), vb( vb ), ib( ib ), mat( mat ), mode( PolygonMode::Solid )
{

}

//-----------------------------------//

Renderable::Renderable( Primitive::Enum primitive, 
						const VertexBufferPtr& vb, 
						const MaterialPtr& mat )
	: type( primitive), vb( vb ), mat( mat ), mode( PolygonMode::Solid )

{

}

//-----------------------------------//

Renderable::~Renderable()
{

}

//-----------------------------------//

void Renderable::render( const render::Device& UNUSED(device) )
{
	if( mat ) mat->bind();

	if( !vb->isBuilt() )
		vb->build();

	vb->bind();

	if( mode == PolygonMode::Wireframe )
		glPolygonMode( GL_FRONT_AND_BACK, PolygonMode::Wireframe );

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
   
	if( mode == PolygonMode::Wireframe )
		glPolygonMode( GL_FRONT_AND_BACK, PolygonMode::Solid );

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

PolygonMode::Enum Renderable::getPolygonMode() const
{
	return mode;
}

//-----------------------------------//

void Renderable::setMaterial( const MaterialPtr& mat )
{
	this->mat = mat;
}

//-----------------------------------//

void Renderable::setPrimitiveType( Primitive::Enum primitive )
{
	this->type = primitive;
}

//-----------------------------------//

void Renderable::setVertexBuffer( const VertexBufferPtr& vb )
{
	this->vb = vb;
}

//-----------------------------------//

void Renderable::setIndexBuffer( const IndexBufferPtr& ib )
{
	this->ib = ib;
}

//-----------------------------------//

void Renderable::setPolygonMode( PolygonMode::Enum mode )
{
	this->mode = mode;
}

//-----------------------------------//

} } // end namespaces