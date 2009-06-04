/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <vapor/render/gl/GL_VertexBuffer.h>

#ifdef VAPOR_RENDERER_OPENGL

namespace vapor {
	namespace render {
		namespace opengl {

//-----------------------------------//

GLVertexBuffer::GLVertexBuffer(uint numElems, VertexDeclaration decl, BufferUsage::Enum bu, BufferType::Enum bt)
		: VertexBuffer(numElems, decl, bu, bt)
{
	if(GLEW_ARB_vertex_buffer_object)
	{
		glGenBuffersARB(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
	} 
	else 
	{
		error("gl::buffers", "Your graphics device does not support Vertex Buffer Objects (VBOs).");
	}
}

//-----------------------------------//

GLVertexBuffer::~GLVertexBuffer()
{

}

//-----------------------------------//

void* GLVertexBuffer::map()
{
	return nullptr;
}

//-----------------------------------//

void GLVertexBuffer::unmap()
{

}

//-----------------------------------//

} } } // end namespaces

#endif