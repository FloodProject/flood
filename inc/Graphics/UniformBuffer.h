/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Graphics/Buffer.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Core/Math/Matrix4x4.h"
#include "Core/Containers/HashMap.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

enum struct UniformDataType : uint8
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

/**
 * Uniforms are named constants that can be set in programs.
 * These are allocated from a special frame bump allocator,
 * that frees all the memory when the frame ends.
 */

struct API_GRAPHICS UniformBufferElement
{
	const char* name;
	UniformDataType type;
	uint16 count;
	uint8 data[1]; // Variable length data.
};

//-----------------------------------//

typedef HashMap<String, UniformBufferElement*> UniformBufferElements;

/**
 * Represents a uniform buffer.
 */

class API_GRAPHICS UniformBuffer : public ReferenceCounted
{
public:
    // Todo: fix bindings generator so that FLD_IGNORE is not needed here
	FLD_IGNORE UniformBufferElements elements;

	// Gets the uniform if it exists or creates a new one.
	UniformBufferElement* getElement(const char* name, size_t size);

	// Removes the named uniform.
	void removeUniform( const char* slot );

	// Adds a uniform to the shader.
	void setUniform( const char* slot, int32 data );

	// Adds a named float uniform to the program.
	void setUniform( const char* slot, float value );

	// Adds a named Vector3 array uniform to the program.
	void setUniform( const char* slot, const Vector<Vector3>& vec );

	// Adds a named color array uniform to the program.
	void setUniform( const char* slot, const Vector<Color>& vec );

	// Adds a named vector uniform to the program.
	void setUniform( const char* slot, const Vector3& vec );

	// Adds a named matrix uniform to the program.
	void setUniform( const char* slot, const Matrix4x3& );

	// Adds a named 4x4 matrix uniform to the program.
	void setUniform( const char* slot, const Matrix4x4& );

	// Adds a named 4x4 matrix vector uniform to the program.
	void setUniform( const char* slot, const Vector<Matrix4x4>& vec );
};

//API_GRAPHICS UniformBuffer* UniformBufferCreate( Allocator* );

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( UniformBuffer );

//-----------------------------------//

NAMESPACE_GRAPHICS_END