/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_RENDERER_OPENGL

#include "Graphics/UniformBuffer.h"
#include "Graphics/Render.h"
#include "Math/Matrix4x3.h"
#include "Math/Matrix4x4.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

static char* AllocateName(const String& name)
{
	static const char* s_EmptyString = "";
	if( name.empty() ) return (char*) s_EmptyString;

	size_t nameSize = sizeof(char)*name.size();
	char* newName = (char*) AllocatorAllocate(GetFrameAllocator(), nameSize+1, 0);
	memcpy(newName, name.c_str(), nameSize);
	newName[nameSize] = '\0';
	return newName;
}

//-----------------------------------//

UniformBufferElement* UniformBuffer::getElement(const String& name, size_t size)
{
	//UniformBufferElements::iterator it = elements.find( name.c_str() );
	//if( it != elements.end() ) return it->second;

	void* p = AllocatorAllocate(GetFrameAllocator(), sizeof(UniformBufferElement)+size, 0);
	UniformBufferElement* element = (UniformBufferElement*) p;

	if( name.empty() )
	{
		LogAssert("Uniform elements should be named");
		return nullptr;
	}

	if( !element ) return nullptr;

	element->type = (UniformDataType::Enum) 0;
	element->count = 0;
	element->name = AllocateName(name);

	elements[name] = element;
	return element;
}

//-----------------------------------//

void UniformBuffer::removeUniform( const String& slot )
{
	UniformBufferElements::iterator it = elements.find(slot);
	if( it == elements.end() ) return;

	elements.erase(it);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& name, int32 data )
{
	size_t size = sizeof(int32);
	UniformBufferElement* element = getElement(name, size);
	if( !element ) return;
	element->type = UniformDataType::Scalar_I;
	element->count = 1;
	memcpy(&element->data, &data, size);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& name, float data )
{
	size_t size = sizeof(float);
	UniformBufferElement* element = getElement(name, size);
	if( !element ) return;
	element->type = UniformDataType::Scalar_F;
	element->count = 1;
	memcpy(&element->data, &data, size);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& name, const std::vector<Vector3>& vec )
{
	size_t size = sizeof(Vector3)*vec.size();
	UniformBufferElement* element = getElement(name, size);
	if( !element ) return;
	element->type = UniformDataType::Vector3_F;
	element->count = vec.size();
	memcpy(&element->data, &vec.front(), size);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& name, const std::vector<Color>& vec )
{
	assert(0 && "Not implemented yet");

#if 0
	size_t size = sizeof(Color)*vec.size();
	UniformBufferElement* element = getElement(name, size);
	if( !element ) return;
	element->type = UniformDataType::Vector3_F;
	element->count = vec.size();
	memcpy(&element->data, &vec.front(), size);
#endif
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& name, const Vector3& vec )
{
	size_t size = sizeof(Vector3);
	UniformBufferElement* element = getElement(name, size);
	if( !element ) return;
	element->type = UniformDataType::Vector3_F;
	element->count = 1;
	memcpy(&element->data, &vec.x, size);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& name, const Matrix4x3& matrix )
{
	Matrix4x4 m(matrix);
	size_t size = sizeof(Matrix4x4);
	UniformBufferElement* element = getElement(name, size);
	if( !element ) return;
	element->type = UniformDataType::Matrix4_F;
	element->count = 1;
	memcpy(&element->data, &m.m11, size);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& name, const Matrix4x4& matrix )
{
	size_t size = sizeof(Matrix4x4);
	UniformBufferElement* element = getElement(name, size);
	if( !element ) return;
	element->type = UniformDataType::Matrix4_F;
	element->count = 1;
	memcpy(&element->data, &matrix.m11, size);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& name, const std::vector<Matrix4x4>& vec )
{
	size_t size = sizeof(Matrix4x4)*vec.size();
	UniformBufferElement* element = getElement(name, size);
	if( !element ) return;
	element->type = UniformDataType::Matrix4_F;
	element->count = vec.size();
	memcpy(&element->data, &vec.front(), size);
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif