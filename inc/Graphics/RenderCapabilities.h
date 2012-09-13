/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#ifdef ENABLE_RENDERER_OPENGL

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * This class represents the graphics capabilities of the video card,
 * and holds data related to it that is interesting for the other classes,
 * like driver information.
 */

class API_GRAPHICS RenderCapabilities
{
public:

	String name;
	String vendor;
	String driverName;
	String driverVersion;
	String shadingLanguageVersion;

	int32 maxTextureSize;
	int32 maxTextureUnits;
	int32 maxAttribs;
	
	bool supportsVertexBuffers;
	bool supportsShaders;
	bool supportsAnisotropic;
	bool supportsFixedPipeline;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END

#endif