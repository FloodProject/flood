/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/VertexBuffer.h"
#include "vapor/render/GL.h"

namespace vapor {
	namespace render {

////-----------------------------------//
//
//VertexBuffer::VertexBuffer(uint numElems, VertexDeclaration decl, BufferUsage::Enum bu, BufferType::Enum bt)
//		: bufferUsage(bu), bufferType(bt), numElements(numElems), vertexDeclaration(decl)
//{
//	if(GLEW_ARB_vertex_buffer_object)
//	{
//		glGenBuffersARB(1, &id);
//		glBindBuffer(GL_ARRAY_BUFFER, id);
//	} 
//	else 
//	{
//	}
//}
//
////-----------------------------------//
//
//VertexBuffer::~VertexBuffer()
//{
//
//}
//
////-----------------------------------//
//
//void* VertexBuffer::map()
//{
//	return nullptr;
//}
//
////-----------------------------------//
//
//void VertexBuffer::unmap()
//{
//
//}
//
////-----------------------------------//

} } // end namespaces

#endif