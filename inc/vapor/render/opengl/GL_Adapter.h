/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

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
