/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Shader.h"

#include "vapor/render/GL.h"

namespace vapor {
	namespace render {

//-----------------------------------//
		
/**
 * GLSL Shader.
 */

class GLSL_Program
{
public:

	GLSL_Program( resources::ShaderType::Enum e, const std::string& text );
	virtual ~GLSL_Program();


};

//-----------------------------------//

} } // end namespaces