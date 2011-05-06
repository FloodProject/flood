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
	enum Enum
	{
		Box = nvtt::MipmapFilter_Box,
		Triangle = nvtt::MipmapFilter_Triangle,
		Kaizer = nvtt::MipmapFilter_Kaiser
	};
};

REFLECT_DECLARE_ENUM(MipmapFilter)

//-----------------------------------//

struct PIPELINE_API CompressionQuality
{
	enum Enum
	{
		Fastest,
		Normal,
		Production,
		Highest,
	};
};

REFLECT_DECLARE_ENUM(CompressionQuality)

//-----------------------------------//

struct PIPELINE_API CompressionFormat
{
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

REFLECT_DECLARE_ENUM(CompressionFormat)

/**
 * Resource processors handle the processing of resource data.
 */

REFLECT_DECLARE_CLASS(ImageProcessor)

class PIPELINE_API ImageProcessor : public ResourceProcessor
{
	REFLECT_DECLARE_OBJECT(ImageProcessor)

public:

	ImageProcessor();
	~ImageProcessor();

	// Processes the given resource.
	bool Process(const ResourcePtr& resource);

	// Gets the processed resource type.
	Class* GetResourceType() { return ImageGetType(); }

	CompressionFormat::Enum format;
	CompressionQuality::Enum quality;
	bool generateMipmaps;
	MipmapFilter::Enum mipmapFilter;
};

//-----------------------------------//

} // end namespace
