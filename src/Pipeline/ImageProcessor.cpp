/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Pipeline/API.h"
#include "Pipeline/ImageProcessor.h"
#include "Log.h"

#if 0
#include <nvtt/nvtt.h>
using namespace nvtt;
#endif

namespace vapor {

//-----------------------------------//

BEGIN_ENUM(MipmapFilter)
	ENUM(Box)
	ENUM(Triangle)
	ENUM(Kaizer)
END_ENUM()

BEGIN_ENUM(CompressionQuality)
	ENUM(Fastest)
	ENUM(Normal)
	ENUM(Production)
	ENUM(Highest)
END_ENUM()

BEGIN_ENUM(CompressionFormat)
	ENUM(RGB)
	ENUM(RGBA)
	ENUM(DXT1)
	ENUM(DXT1a)
	ENUM(DXT3)
	ENUM(DXT5)
	ENUM(DXT5nm)
END_ENUM()

BEGIN_CLASS_PARENT(ImageProcessor, ResourceProcessor)
	FIELD_ENUM(CompressionFormat, format)
	FIELD_ENUM(CompressionQuality, quality)
	FIELD_PRIMITIVE(bool, generateMipmaps)
	FIELD_ENUM(MipmapFilter, mipmapFilter)
END_CLASS()

//-----------------------------------//

ImageProcessor::ImageProcessor()
	: format(CompressionFormat::DXT5)
	, quality(CompressionQuality::Normal)
	, generateMipmaps(true)
	, mipmapFilter(MipmapFilter::Box)
{
	static bool once = false;
	if( !once )
		//LogInfo("Using NVIDIA Texture Tools %u", nvtt::version());
	once = true;
}

//-----------------------------------//

ImageProcessor::~ImageProcessor() { }

//-----------------------------------//

struct ImageOutputHandler : public nvtt::OutputHandler
{
	void beginImage(int size, int width, int height, int depth, int face, int miplevel);
	bool writeData(const void * data, int size);
};

bool ImageProcessor::Process(const ResourcePtr& resource)
{
	if( resource->getResourceGroup() != ResourceGroup::Images )
		return false;

	const ImagePtr& image = RefCast<Image>(resource);

#if 0
	InputOptions input;
	input.setMipmapGeneration(generateMipmaps);
	input.setMipmapFilter((nvtt::MipmapFilter) mipmapFilter);

	CompressionOptions compression;
	compression.setQuality((nvtt::Quality) quality);
	compression.setFormat((nvtt::Format) format);

	ImageOutputHandler handler;

	OutputOptions output;
	output.setOutputHandler(&handler);

	Compressor compressor;
	
	if( !compressor.process(input, compression, output) )
		return false;
#endif
	return true;
}

//-----------------------------------//

void ImageOutputHandler::beginImage(int size, int width, int height, int depth, int face, int miplevel)
{

}

//-----------------------------------//

bool ImageOutputHandler::writeData(const void * data, int size)
{
	return false;
}

//-----------------------------------//

} // end namespace