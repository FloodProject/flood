/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Target.h"

namespace vapor { namespace render {

//-----------------------------------//

/**
 * FBOs, also known as framebuffer objects, are render targets that you
 * can render to. These are useful when you want to render from another
 * viewpoint and combine the contents in the main render target or even
 * render the scene and apply custom post-processing effects. They are
 * also used to render depth from a viewpoint, which is how regular 
 * shadow mapping works.
 */

class VAPOR_API FBO : public RenderTarget
{
public:

	FBO (const Settings& settings);
	virtual ~FBO ();

	// Updates the render target (usually swaps buffers).
	virtual void update();

	// Sets this rendering target as the current.
	virtual void makeCurrent();

	// Gets the settings of this render target.
	virtual const Settings& getSettings();

protected:

	// Holds the window settings
	Settings settings;
};

//-----------------------------------//

} } // end namespaces