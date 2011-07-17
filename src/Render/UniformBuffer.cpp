/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "Render/UniformBuffer.h"
#include "Render/Render.h"
#include "Math/Matrix4x3.h"
#include "Math/Matrix4x4.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

static UniformBufferElement* CreateUniformElement(size_t size, const String& name)
{
	UniformBufferElement* element = (UniformBufferElement*)
		AllocatorAllocate(GetFrameAllocator(),
		sizeof(UniformBufferElement)+size, 0);

	element->name = nullptr;
	element->type = (UniformDataType::Enum) 0;
	element->count = 0;

	char* newName = (char*) AllocatorAllocate(
		GetFrameAllocator(), sizeof(char)*name.size()+1, 0);

	memcpy(newName, name.c_str(), name.size());
	newName[name.size()] = '\0';

	element->name = newName;

	return element;
};

//-----------------------------------//

void UniformBuffer::setUniform( const String& slot, int32 data )
{
	size_t size = sizeof(int32);
	UniformBufferElement* element = CreateUniformElement(size, slot);
	element->type = UniformDataType::Scalar_I;
	element->count = 1;
	memcpy(&element->data, &data, size);
	elements.push_back(element);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& slot, float data )
{
	size_t size = sizeof(float);
	UniformBufferElement* element = CreateUniformElement(size, slot);
	element->type = UniformDataType::Scalar_F;
	element->count = 1;
	memcpy(&element->data, &data, size);
	elements.push_back(element);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& slot, const std::vector<Vector3>& vec )
{
	size_t size = sizeof(Vector3)*vec.size();
	UniformBufferElement* element = CreateUniformElement(size, slot);
	element->type = UniformDataType::Vector3_F;
	element->count = vec.size();
	memcpy(&element->data, &vec.front(), size);
	elements.push_back(element);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& slot, const std::vector<Color>& vec )
{
	assert(0 && "Not implemented yet");

#if 0
	size_t size = sizeof(Color)*vec.size();
	UniformBufferElement* element = CreateUniformElement(size, slot);
	element->type = UniformDataType::Vector3_F;
	element->count = vec.size();
	memcpy(&element->data, &vec.front(), size);
	elements.push_back(element);
#endif
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& slot, const Vector3& vec )
{
	size_t size = sizeof(Vector3);
	UniformBufferElement* element = CreateUniformElement(size, slot);
	element->type = UniformDataType::Vector3_F;
	element->count = 1;
	memcpy(&element->data, &vec.x, size);
	elements.push_back(element);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& slot, const Matrix4x3& matrix )
{
	Matrix4x4 m(matrix);
	size_t size = sizeof(Matrix4x4);
	UniformBufferElement* element = CreateUniformElement(size, slot);
	element->type = UniformDataType::Matrix4_F;
	element->count = 1;
	memcpy(&element->data, &m.m11, size);
	elements.push_back(element);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& slot, const Matrix4x4& matrix )
{
	size_t size = sizeof(Matrix4x4);
	UniformBufferElement* element = CreateUniformElement(size, slot);
	element->type = UniformDataType::Matrix4_F;
	element->count = 1;
	memcpy(&element->data, &matrix.m11, size);
	elements.push_back(element);
}

//-----------------------------------//

void UniformBuffer::setUniform( const String& slot, const std::vector<Matrix4x4>& vec )
{
	size_t size = sizeof(Matrix4x4)*vec.size();
	UniformBufferElement* element = CreateUniformElement(size, slot);
	element->type = UniformDataType::Matrix4_F;
	element->count = vec.size();
	memcpy(&element->data, &vec.front(), size);
	elements.push_back(element);
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif