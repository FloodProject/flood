/************************************************************************
*
* vapor3D © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "ResourceProcessor.h"
#include "Resources/Image.h"
#include <nvtt/nvtt.h>

namespace vapor {

//-----------------------------------//

struct PIPELINE_API MipmapFilter
{
	DECLARE_ENUM()

	enum Enum
	{
		Box = nvtt::MipmapFilter_Box,
		Triangle = nvtt::MipmapFilter_Triangle,
		Kaizer = nvtt::MipmapFilter_Kaiser
	};
};

//-----------------------------------//

struct PIPELINE_API CompressionQuality
{
	DECLARE_ENUM()

	enum Enum
	{
		Fastest,
		Normal,
		Production,
		Highest,
	};
};

//-----------------------------------//

struct PIPELINE_API CompressionFormat
{
	DECLARE_ENUM()

	enum Enum
	{
		RGB,
		RGBA,
		DXT1,
		DXT1a,
		DXT3,
		DXT5,
		DXT5nm
	};
};

/**
 * Resource processors handle the processing of resource data.
 */

class PIPELINE_API ImageProcessor : public ResourceProcessor
{
	DECLARE_CLASS_()

public:

	ImageProcessor();
	~ImageProcessor();

	// Processes the given resource.
	bool Process(const ResourcePtr& resource);

	// Gets the processed resource type.
	const Class& GetResourceType() { return Image::getStaticType(); }

protected:

	CompressionFormat::Enum format;
	CompressionQuality::Enum quality;
	bool generateMipmaps;
	MipmapFilter::Enum mipmapFilter;
};

//-----------------------------------//

} // end namespace
