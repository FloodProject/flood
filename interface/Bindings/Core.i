/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

%include <stl.i>
%include "csharpaddons.i"

// Fix all enums by renaming them to the simplified type.
//%rename("%(regex:/(\\w+)::Enum/\\1/)s") ""; // Name::Enum -> Name

%define API_CORE
%enddef

%import "Core/API.h"
%import "Core/Platform.h"
%import "Core/String.h"

%apply std::string { String, Path };

//%import "Core/Utilities.h"
//%import "Core/Concurrency.h"
//%import "Core/References.h"

class ReferenceCounted;

API_CORE void LogDebug(const char* msg, ...);
API_CORE void LogInfo(const char* msg, ...);
API_CORE void LogWarn(const char* msg, ...);
API_CORE void LogError(const char* msg, ...);

%define REFLECT_DECLARE_CLASS(arg)
%enddef

%define REFLECT_DECLARE_ENUM(arg)
%enddef

%define REFLECT_DECLARE_OBJECT(arg)
%enddef

%define DECLARE_FRIENDS(arg)
%enddef

%define OVERRIDE
%enddef

%define TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(arg)
%enddef

%define FWD_DECL_INTRUSIVE(arg)
%enddef

// TODO: enums, events/delegates, handles


