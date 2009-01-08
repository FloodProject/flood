/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Engine.h"

namespace vapor {
	namespace resources {

class PNG
{
	ulong width, height;
	vector<byte> buffer;

	void decode(const string& filename);

public:

	PNG(const string& filename);

	const int getWidth() const;
	const int getHeight() const;

	vector<byte>& getBuffer();
};

} } // end namespaces
