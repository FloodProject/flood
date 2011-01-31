/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"
#include "math/Color.h"

namespace vapor {

//-----------------------------------//

/**
 * Possible pixel formats for an image.
 */

struct PixelFormat 
{
	enum Enum 
	{
		// Uncompressed formats
		R8G8B8A8 = 0,
		R8G8B8,
		B8G8R8,
		B8G8R8A8,

		// Compressed formats
		DXT1,
		DXT1a,
		DXT3,
		DXT5,
		DXT5nm,

		// Misc. formats
		Depth,
		Unknown
	};

	DECLARE_ENUM()
};

//-----------------------------------//

/**
 * Represents an image in the engine. An image usually holds uncompressed 
 * image data, stored in an pixel array in a given pixel format specified 
 * at creation time.
 */

class RESOURCE_API Image : public Resource
{
	DECLARE_CLASS_()

public:

	Image();
	Image(ushort width, ushort height, PixelFormat::Enum format);

	// Gets/sets the width of the image.
	ACESSOR(Width, const uint, width)
	
	// Gets/sets the height of the image.
	ACESSOR(Height, const uint, height)

	// Gets/sets the pixel format of the image.
	ACESSOR(PixelFormat, PixelFormat::Enum, format)

	// Gets/sets the buffer containing the image data.
	ACESSOR(Buffer, const std::vector<byte>&, buffer)

	// Returns if the image is in a compressed format.
	bool isCompressed() const;

	// Gets/sets the buffer containing the image data.
	std::vector<byte>& getBuffer() { return buffer; }

	// Return the proper resource group for this resource.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Images)

	// Sets the image with the given color.
	void setColor( const Color& color );

	// Prints image information to the log.
	void log() const;

private:

	// Image width.
	uint width;

	// Image height.
	uint height;

	// Pixel format.
	PixelFormat::Enum format;

	// Image data.
	std::vector<byte> buffer;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Image );

class RESOURCE_API ImageWriter
{
public:

	// Saves the image contents to a file.
	void save( const ImagePtr& image, const std::string& filename );

	bool convertPNG( const ImagePtr& image );

protected:

	std::vector<byte> output;
};

//-----------------------------------//

} // end namespace