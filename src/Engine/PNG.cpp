/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <iostream>
#include <fstream>

#include "vapor/resources/PNG.h"

int decodePNG(std::vector<byte>& out_image_32bit, ulong& image_width, 
			  ulong& image_height, const byte* in_png, ulong in_size);

namespace vapor {
	namespace resources {

PNG::PNG(const string& filename)
{
	decode(filename);
}

vector<byte>& PNG::getBuffer()
{
	return buffer;
}

void PNG::decode(const string& filename)
{
	vector<byte> filebuf;
	ifstream file(filename.c_str(), ios::in | ios::binary | ios::ate);

	//get filesize
	streamsize size = 0;
	if(file.seekg(0, ios::end).good()) size = file.tellg();
	if(file.seekg(0, ios::beg).good()) size -= file.tellg();

	//read contents of the file into the vector
	filebuf.resize(size_t(size));
	if(size > 0) file.read((char*)(&filebuf[0]), size);

	decodePNG(buffer, width, height, &filebuf[0], (ulong) filebuf.size());
}

const int PNG::getWidth() const
{
	return width;
}

const int PNG::getHeight() const
{
	return height;
}

} } // end namespaces
