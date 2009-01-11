/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org/
*
************************************************************************/

#pragma once

#include "vapor/Engine.h"

class IndexBuffer
{
public:
	IndexBuffer(void);
	~IndexBuffer(void);

	bool is32bitIndexed();

private:

	bool _is32bit;
};
