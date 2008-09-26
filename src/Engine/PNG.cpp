/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//// 
//// fuckEngine by triton (2008)
////
//// License: WTF Public License
////
//// Thanks to:
////	MaulingMonkey @ #gamedev
////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <iostream>
#include <fstream>

#include "PNG.h"

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

SDL_Surface* convertPNGToSurface(PNG &png)
{
	SDL_Surface* surf;
	
	surf = SDL_CreateRGBSurfaceFrom ( &(png.getBuffer()[0]), png.getWidth(), 
		png.getHeight(), 32, png.getWidth()*4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	return surf;
}

} } // end namespaces