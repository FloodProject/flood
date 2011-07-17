/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_RENDERER_OPENGL

namespace vapor {

//-----------------------------------//	

/**
 * The adapter class just represents a graphics adapter (video card), 
 * and holds data related to it that is interesting for the other classes,
 * like driver information and capabilities. Obviously this depends on the
 * rendering API the app is using at runtime, but I still have to check if
 * we need different classes for different render devices. Hopefully not. :)
 */

class API_ENGINE Adapter
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

} // end namespace

#endif