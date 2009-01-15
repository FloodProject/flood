/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/opengl/GL_Device.h"
#include "vapor/render/opengl/GL_Adapter.h"

#ifdef VAPOR_RENDERER_OPENGL

namespace vapor {
	namespace render {
		namespace opengl {

GLVertexBuffer::GLVertexBuffer()
{
	//VBO ID set to 0
	_ID = 0;
};

GLVertexBuffer::~GLVertexBuffer()
{
	destroy();
};

void GLVertexBuffer::destroy()
{
	//If the bufffer is valid, delete it.
	if(GLEW_ARB_vertex_buffer_object && glIsBufferARB(_ID))
	{
		glDeleteBuffersARB(1, (const GLuint*)&_ID);
		_ID = 0;
	};
};

void* GLVertexBuffer::map(unsigned long Usage)
{
	//If we're using VBOs
	if(GLEW_ARB_vertex_buffer_object && glIsBufferARB(_ID))
	{
		//Bind the buffer
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _ID);
		
		//Convert from vaporFlag to glFlag
		switch(Usage)
		{
		case BufferAccessUsage::READ_ONLY:
			Usage = GL_READ_ONLY;
			break;
		case BufferAccessUsage::WRITE_ONLY:
			Usage = GL_WRITE_ONLY;
			break;
		case BufferAccessUsage::READ_WRITE:
			Usage = GL_READ_WRITE;
			break;
		default:
			//Shit happened.
			info("render::opengl::vertexbuffer::map", "Invalid usage specified");
			return 0;
		};

		//Try to map the buffer
		void* Ptr = glMapBufferARB(GL_ARRAY_BUFFER_ARB, Usage);
		if(!Ptr)
		{
			//If we couldnt map it, get the currently mapped pointer 
			//(this works most of the times if we get a null ptr)
			glGetBufferPointerv(GL_ARRAY_BUFFER, GL_BUFFER_MAP_POINTER, (GLvoid**)&Ptr);
			if(!Ptr)
			{
				//If we failed, tell us
				info("render::opengl::vertexbuffer::map", 
					"Failed to obtain the mapping pointer for this vertex buffer");
			};
		};
		//Return our pointer (could be null)
		return Ptr;
	}
	else if(Data.size() != 0) //Else if the VB is valid
	{
		//Return a pointer to it's data
		return &Data[0];
	};
	//Else the VB is invalid, return null
	return 0;
};

void GLVertexBuffer::unmap()
{
	//If our buffer is real, bind it, unmap it, and unbind it.
	if(GLEW_ARB_vertex_buffer_object && glIsBufferARB(_ID))
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _ID);
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	};
};

void GLVertexBuffer::create(const void* Content, unsigned long ContentLength, unsigned long CreationUsage)
{
	destroy();

	//If we have no content, exit.
	if(ContentLength == 0)
	{
		info("render::opengl::vertexbuffer::create", "Invalid content length");
		return;
	};

	//If we can make a VBO
	if(ContentLength > 5120 && GLEW_ARB_vertex_buffer_object) //5 KB
	{
		//Convert vaporFlag to glFlag
		switch(CreationUsage)
		{
		case BufferCreationUsage::STREAM_DRAW:
			CreationUsage = GL_STREAM_DRAW;
			break;
		case BufferCreationUsage::STREAM_READ:
			CreationUsage = GL_STREAM_READ;
			break;
		case BufferCreationUsage::STREAM_COPY:
			CreationUsage = GL_STREAM_COPY;
			break;
		case BufferCreationUsage::STATIC_DRAW:
			CreationUsage = GL_STATIC_DRAW;
			break;
		case BufferCreationUsage::STATIC_READ:
			CreationUsage = GL_STATIC_READ;
			break;
		case BufferCreationUsage::STATIC_COPY:
			CreationUsage = GL_STATIC_COPY;
			break;
		case BufferCreationUsage::DYNAMIC_DRAW:
			CreationUsage = GL_DYNAMIC_DRAW;
			break;
		case BufferCreationUsage::DYNAMIC_READ:
			CreationUsage = GL_DYNAMIC_READ;
			break;
		case BufferCreationUsage::DYNAMIC_COPY:
			CreationUsage = GL_DYNAMIC_COPY;
			break;
		default:
			//Shit happened.
			info("render::opengl::vertexbuffer::create", "Invalid Creation Usage");
			return;
			break;
		};

		//Generate a buffer
		glGenBuffersARB(1, (GLuint*)&_ID);
		//Bind it
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _ID);
		//Set it's data
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, ContentLength, Content, CreationUsage);
		//Unbind it
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}
	else
	{
		//Reset the Data size
		Data.resize(ContentLength);
		//If we have a content pointer
		if(Content)
		{
			//Copy it
			memcpy(&Data[0], Content, ContentLength);
		};
	};
};

void GLVertexBuffer::setVertexElements(VertexElement Elements[])
{
	//Clear our current elements
	_elements.clear();
	//Copy all elements
	unsigned long CurrentOffset = 0;
	for(unsigned long i = 0; i < sizeof(Elements) / sizeof(VertexElement); i++)
	{
		//Final usage, quit (redundant much?)
		if(Elements[i].Usage == VertexUsage::NONE)
			break;
		//Check if the offsets are correct
		if(i > 0)
		{
			//Invalid offset
			if(Elements[i].Offset != CurrentOffset)
			{
				info("render::opengl::vertexbuffer::setvertexelements",
					"Invalid offset found for an element");
				_elements.clear();
				return;
			};

			//Increase the offset
			switch(Elements[i].DataType)
			{
			case VertexDataType::FLOAT1:
				CurrentOffset += sizeof(float);
				break;
			case VertexDataType::FLOAT2:
				CurrentOffset += sizeof(float[2]);
				break;
			case VertexDataType::FLOAT3:
				CurrentOffset += sizeof(float[3]);
				break;
			case VertexDataType::FLOAT4:
				CurrentOffset += sizeof(float[4]);
				break;
			};
		}
		else
		{
			//Invalid initial element
			if(Elements[i].Offset != 0)
			{
				info("render::opengl::vertexbuffer::setvertexelements",
					"Invalid first element offset");
			};

			switch(Elements[i].DataType)
			{
			case VertexDataType::FLOAT1:
				CurrentOffset = sizeof(float);
				break;
			case VertexDataType::FLOAT2:
				CurrentOffset = sizeof(float[2]);
				break;
			case VertexDataType::FLOAT3:
				CurrentOffset = sizeof(float[3]);
				break;
			case VertexDataType::FLOAT4:
				CurrentOffset = sizeof(float[4]);
				break;
			};
		};
		//Add it
		_elements.push_back(Elements[i]);
	};
};

} } }
#endif
