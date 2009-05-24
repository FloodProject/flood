/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/render/Buffer.h"

namespace vapor {
	namespace render {

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(void);
	~IndexBuffer(void);

	bool is32bitIndexed();

private:

	bool _is32bit;
};

} } // end namespaces
