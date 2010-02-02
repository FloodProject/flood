/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Font.h"
#include "vapor/render/Material.h"
#include "vapor/scene/Geometry.h"
#include "vapor/gui/Overlay.h"

namespace vapor { namespace gui {

//-----------------------------------//

/**
 * Scripting console shown in-game to control the engine at runtime via.
 * text commands. It has a scrolling list of text and also a text input 
 * box where you input a command and press <Enter>. The engine (Lua) will
 * evaluate it and execute the action. As this will be based on the Lua
 * scripting engine, you can pretty much evaluate anything that is bound
 * to its environment.
 */

class VAPOR_API Console : public gui::Overlay
{
public:

	Console();
	virtual ~Console();

	// Updates the console geometry if needed.
	virtual void update( double delta );

	// Serializes this node to a stream.
	virtual const std::string save( int indent = 0 );

	// Returns this component name identification.
	virtual const std::string& getType() const;

protected:

	// Builds the initial geometry of the console.
	void buildGeometry();
	
	// Used to track if the geometry needs updating.
	bool isDirty;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Console );

//-----------------------------------//

} } // end namespaces