/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

%module vapor

%begin %{
#include "vapor/PCH.h"
#include "vapor/Engine.h"
%}

%import "base.i"

namespace vapor
{
	VAPOR_API void Log::debug(const std::string& msg);
	VAPOR_API void info(const std::string& subsystem, const char* msg, ...);
	VAPOR_API void warn(const std::string& subsystem, const char* msg, ...);
	VAPOR_API void error(const std::string& subsystem, const char* msg, ...);
}

%include "math.i"
%include "resources.i"
%include "input.i"
%include "render.i"
%include "scene.i"

//%include "vapor/Engine.h"
