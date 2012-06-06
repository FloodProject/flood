/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

%module FlushEngine

%begin %{
#include "Engine/API.h"
//#include "Engine/Headers.h"
//using namespace Flush;
%}

%rename("%(firstuppercase)s") "";

%include "Core.i"
//%include "Math.i"
//%include "Resources.i"
%include "Render.i"
//%include "Scene.i"
//%include "Input.i"

%define NAMESPACE_ENGINE_BEGIN(arg)
%enddef

//%include "Engine/Engine.h"