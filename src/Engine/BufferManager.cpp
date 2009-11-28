/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/BufferManager.h"
#include "vapor/render/GL.h"

namespace vapor { namespace render {

//-----------------------------------//

BufferManager::BufferManager() 
{

}

//-----------------------------------//

BufferManager::~BufferManager() 
{

}

//-----------------------------------//

//shared_ptr<VertexBuffer> BufferManager::createVertexBuffer(uint numElements, 
//		VertexDeclaration decl, BufferUsage::Enum bue, BufferType::Enum bte)
//{
//	VertexBuffer *buffer = new VertexBuffer(numElements, decl, bue, bte);
//	vertexBuffers.push_back(buffer);
//
//	shared_ptr<VertexBuffer> ptr(vertexBuffers.back());
//	return ptr;
//}

//-----------------------------------//

//shared_ptr<IndexBuffer> GLBufferManager::createIndexBuffer()
//{
//	//GLVertexBuffer Buffer;
//	//_vertexBuffers.push_back(Buffer);
//	//return &_vertexBuffers.back();
//	shared_ptr<IndexBuffer> ptr(nullptr);
//	return ptr;
//}

//-----------------------------------//

//void GLDevice::bindVertexBuffer(VertexBuffer *Buffer)
//{
	//if(Buffer) //If valid
	//{
	//	//Search for the real buffer (user may mistakenly use a non-GL buffer)
	//	GLVertexBuffer *RealBuffer = 0;
	//	for(std::list<GDVertexBuffer>::iterator it = _vertexBuffers.begin(); 
	//		it != _vertexBuffers.end(); it++)
	//	{
	//		if(it._Ptr == Buffer)
	//		{
	//			RealBuffer = it._Ptr;
	//			break;
	//		};
	//	};

	//	if(RealBuffer == 0) //if we didnt find it, ignore this operation
	//	{
	//		info("render::opengl::bindvertexbuffer", 
	//			"Invalid buffer submitted for binding");
	//		return;
	//	};

	//	//if the previous buffer was a VBO, we have to unbind it.
	//	if(_bindedVB && GLEW_ARB_vertex_buffer_object && glIsBufferARB(_bindedVB->ID))
	//	{
	//		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	//	};

	//	//Bind the buffer
	//	if(GLEW_ARB_vertex_buffer_object && glIsBufferARB(RealBuffer->_ID))
	//	{
	//		glBindBufferARB(GL_ARRAY_BUFFER_ARB, RealBuffer->_ID);
	//	};

	//	//reference our buffer
	//	_bindedVB = RealBuffer;
	//}
	//else //If we don't 
	//{
	//	//Unbind buffer (doesnt matter whether the last binded VB was a VBO or not)
	//	if(GLEW_ARB_vertex_buffer_object)
	//	{
	//		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	//	};

	//	//reference our buffer
	//	_bindedVB = 0;
	//}
//}

//void GLDevice::bindIndexBuffer(IndexBuffer *Buffer)
//{
//}

//void GLDevice::draw(unsigned long Mode, unsigned long First, unsigned long Count)
//{
//	if(_bindedVB == 0)
//	{
//		info("render::opengl::draw", "Invalid binded VB");
//		return;
//	}
//
//	if(_bindedVB->_elements.size() == 0)
//	{
//		info("render::opengl::draw", "Binded VB has no Vertex Elements");
//		return;
//	};
//
//	//Convert vaporFlag to glFlag
//	switch(Mode)
//	{
//	case Primitive::TRIANGLES:
//		Mode = GL_TRIANGLES;
//		break;
//	case Primitive::LINES:
//		Mode = GL_LINES;
//		break;
//	};
//}

} } // end namespaces

#endif