/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Font.h"
#include "Resources/Material.h"
#include "Scene/Geometry.h"
#include "GUI/Overlay.h"

namespace vapor {

//-----------------------------------//

/**
 * Scripting console shown in-game to control the engine at runtime via.
 * text commands. It has a scrolling list of text and also a text input 
 * box where you input a command and press <Enter>. The engine (Lua) will
 * evaluate it and execute the action. As this will be based on the Lua
 * scripting engine, you can pretty much evaluate anything that is bound
 * to its environment.
 */

REFLECT_DECLARE_CLASS(Console)

class VAPOR_API Console : public Overlay
{
	REFLECT_DECLARE_OBJECT(Console)

public:

	Console();

	// Updates the console geometry if needed.
	virtual void update( float delta );

protected:

	// Builds the initial geometry of the console.
	void buildGeometry();
	
	// Used to track if the geometry needs updating.
	bool isDirty;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Console );

//-----------------------------------//

} // end namespace