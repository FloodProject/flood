/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/Adapter.h"

namespace vapor {
	namespace render {
		namespace opengl {

class GLAdapter : public render::Adapter
{

public:

	GLAdapter();
	virtual ~GLAdapter();

};

} } } // end namespaces

#endif