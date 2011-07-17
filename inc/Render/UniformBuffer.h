/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Render/Buffer.h"
#include "Math/Vector.h"
#include "Math/Color.h"
#include "Math/Matrix4x4.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

namespace UniformDataType
{
	enum Enum
	{
		Scalar_F,
		Scalar_I,
		Vector2_F,
		Vector3_F,
		Matrix2_F,
		Matrix3_F,
		Matrix4_F,
		Matrix2x3_F,
		Matrix3x2_F,
		Matrix2x4_F,
		Matrix4x2_F,
		Matrix3x4_F,
		Matrix4x3_F
	};
};

/**
 * Uniforms are named constants that can be set in programs.
 * These are allocated from a special frame bump allocator,
 * that frees all the memory when the frame ends.
 */

struct API_RENDER UniformBufferElement
{
	char* name;
	UniformDataType::Enum type;
	uint16 count;
	uint8 data[1]; // Variable length data.
};

//-----------------------------------//

typedef std::vector<UniformBufferElement*> UniformBufferElements;

/**
 * Represents a uniform buffer.
 */

class API_RENDER UniformBuffer : public ReferenceCounted
{
public:

	UniformBufferElements elements;

	// Adds a uniform to the shader.
	void setUniform( const String& slot, int32 data );

	// Adds a named float uniform to the program.
	void setUniform( const String& slot, float value );

	// Adds a named float array uniform to the program.
	void setUniform( const String& slot, const std::vector<float> vec );

	// Adds a named Vector3 array uniform to the program.
	void setUniform( const String& slot, const std::vector<Vector3>& vec );

	// Adds a named color array uniform to the program.
	void setUniform( const String& slot, const std::vector<Color>& vec );

	// Adds a named vector uniform to the program.
	void setUniform( const String& slot, const Vector3& vec );

	// Adds a named matrix uniform to the program.
	void setUniform( const String& slot, const Matrix4x3& );

	// Adds a named 4x4 matrix uniform to the program.
	void setUniform( const String& slot, const Matrix4x4& );

	// Adds a named 4x4 matrix vector uniform to the program.
	void setUniform( const String& slot, const std::vector<Matrix4x4>& vec );
};

//API_RENDER UniformBuffer* UniformBufferCreate( Allocator* );

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( UniformBuffer );

//-----------------------------------//

NAMESPACE_ENGINE_END