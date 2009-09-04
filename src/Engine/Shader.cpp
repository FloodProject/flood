/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/resources/Shader.h"

namespace vapor {
	namespace resources {

//-----------------------------------//

namespace ShaderType
{
	const std::string getString(Enum e) 
	{
		switch(e) 
		{
		case Vertex:
			return "Vertex";
		case Fragment:
			return "Fragment";
		case Geometry:
			return "Geometry";
		default:
			return "";
		}
	}
}

//-----------------------------------//


//-----------------------------------//

} } // end namespaces