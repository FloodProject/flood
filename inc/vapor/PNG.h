/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//// 
//// fuckEngine by triton (2008)
////
//// License: WTF Public License
////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Engine.h"

int decodePNG(std::vector<byte>& out_image_32bit, ulong& image_width, 
		  ulong& image_height, const byte* in_png, ulong in_size);

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