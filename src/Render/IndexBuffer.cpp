/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_RENDERER_OPENGL

#include "Render/IndexBuffer.h"
#include "Render/GL.h"
#include "Geometry/GeometryBuffer.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

IndexBuffer::IndexBuffer()
	: isBuilt(false)
{ }

//-----------------------------------//

bool IndexBuffer::bind()
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id );

	if( CheckLastErrorGL( "Error binding index buffer" ) )
		return false;

	return true;
}

//-----------------------------------//

bool IndexBuffer::unbind()
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	if( CheckLastErrorGL( "Error unbinding index buffer" ) )
		return false;

	return true;
}

//-----------------------------------//

bool IndexBuffer::build(const GeometryBufferPtr& gb)
{
	if( !gb ) return false;
	assert( gb->isIndexed() );

	GLsizeiptr indexSize = gb->indexData.size();
	if( indexSize == 0 ) return false;

	bind();

	const GLvoid* data = &gb->indexData.front();

	// Reserve space for all the buffer elements.
	GLenum usage = ConvertBufferGL(gb->getBufferUsage(), gb->getBufferAccess());
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexSize, data, usage );

	if( CheckLastErrorGL("Could not buffer data in index buffer") )
		return false;

	isBuilt = true;
	return true;
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif
