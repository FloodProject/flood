/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Graphics/API.h"
#include "Resources/Resource.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Shaders are used to program the GPU programmable rendering pipeline.
 * It is a set of software instructions that instruct how to calculate
 * each vertex and each pixel processed by the graphics card. We only
 * deal with high-level shaders in the engine, which are compiled to
 * low-level assembly code by the graphics driver.
 */

API_GRAPHICS REFLECT_DECLARE_CLASS(ShaderMaterial)

class API_GRAPHICS ShaderMaterial : public Resource
{
	REFLECT_DECLARE_OBJECT(ShaderMaterial)

public:

	// Returns the vertex shader source code.
	virtual const char* getVertexSource() const = 0;

	// Returns the fragment shader source code.
	virtual const char* getFragmentSource() const = 0;

	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Shaders)

protected:

	ShaderMaterial();
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( ShaderMaterial );
TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( ShaderMaterial );

//-----------------------------------//

NAMESPACE_GRAPHICS_END