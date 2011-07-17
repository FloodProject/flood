/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Shader.h"
#include "Render/VertexBuffer.h"
#include "Math/Matrix4x3.h"
#include "Math/Matrix4x4.h"
#include "Math/EulerAngles.h"
#include "Math/Color.h"

FWD_DECL_INTRUSIVE( UniformBuffer )

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * A program is a collection of shaders (though it has to have at least
 * a vertex shader and a pixel shader). When it is linked it is ready to
 * be used by the renderer. To be useful, a program needs to have some 
 * engine parameters bound, like world matrices and lighting information.
 */

class API_RENDER Program : public ReferenceCounted
{
public:

	virtual ~Program() {}

	// Creates the program.
	virtual bool create() = 0;

	// Creates the shaders and adds them to the program.
	virtual void createShaders() = 0;

	// Updates the shaders text with the program text.
	virtual void updateShadersText() = 0;

	// Links the program and returns if it was successful.
	virtual bool link() = 0;

	// Binds the program to be used for rendering operations.
	virtual void bind() = 0;

	// Unbinds the program.
	virtual void unbind() = 0;

	// Returns if this program has been successfully linked.
	virtual bool isLinked() const;

	// Gets the linking log.
	GETTER(Log, const String&, log)

	// Validates that the program contains valid shaders.
	bool validateShaders() const;

	// Gets the vertex shader in the program.
	GETTER(VertexShader, const RefPtr<Shader>&, vertex)

	// Gets the fragment shader in the program.
	GETTER(FragmentShader, const RefPtr<Shader>&, fragment)

	// Sets the attribute in the program.
	virtual void setAttribute( const String& slot, VertexAttribute::Enum attribute ) = 0;

	// Sets the uniforms in the program.
	virtual void setUniforms( const UniformBufferPtr& ub ) = 0;

protected:

	Program();

	RefPtr<Shader> vertex;
	RefPtr<Shader> fragment;

	String log;
	bool linked;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Program );

//-----------------------------------//

NAMESPACE_ENGINE_END