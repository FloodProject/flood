/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Geometry.h"
#include "vapor/math/Vector2.h"

namespace vapor { namespace gui {

//-----------------------------------//

/**
 * Each overlay can be positioned in two different ways: Relative-mode
 * positioning and Absolute-mode positioning. In Relative-mode you can
 * set an anchor (see the Anchor enum documentation for detailed info
 * about how it works and the different modes you can choose) and the
 * overlay position will be automatically adjusted by the engine even
 * if the resolution changes. So imagine you anchor it to the top left
 * and set a 5-pixel x-offset from it, then it will always stay there.
 */ 

namespace Positioning
{
	enum Enum
	{
		Relative,
		Absolute
	};
}

//-----------------------------------//

/**
 * Anchor points are used for Relative-mode positioning of the overlay. 
 * When you specify an alignment, the positioning of the overlay will be
 * relative to the point you specified. Here is some awesome ASCII art 
 * exemplifying it all:
 *		 ____________________
 *		|                    |
 *		| TL      TC      TR |
 *		|                    |
 *		| L       C        R |
 *		|                    |
 *		| BL      BC      BR |
 *		|____________________|
 *	
 * Hope you understand. :)
 */ 

namespace Anchor
{
	enum Enum
	{
		TopLeft,
		TopCenter,
		TopRight,
		Right,
		BottomRight,
		BottomCenter,
		BottomLeft,
		Left,
		Center,
	};
}

//-----------------------------------//

/**
 * An overlay is a screen-space element that can be used for rendering
 * all kinds of visual elements on the screen, for example GUI widgets.
 * Each overlay can be positioned in two different ways: Relative-mode
 * positioning and Absolute-mode positioning. For mode information on
 * these different positioning modes, please check out the docs above.
 */

class VAPOR_API Overlay : public virtual scene::Geometry
{
public:

	Overlay();

	// Gets/sets the current positioning mode.
	IMPLEMENT_ACESSOR(Positioning, Positioning::Enum, positioning) 

	// Gets/sets the current anchor settings.
	IMPLEMENT_ACESSOR(Anchor, Anchor::Enum, anchor)

	// Gets/sets the positioning of the label.
	IMPLEMENT_ACESSOR(Position, const Vector2i&, position)

	// Sets the position of the overlay.
	void setPosition( int x, int y );

	// Updates the overlay if needed.
	virtual void update( double delta );

	// Returns this component name identification.
	IMPLEMENT_GETTER(Type, const std::string&, type)

protected:

	// Positioning mode used.
	Positioning::Enum positioning;

	// Anchoring mode used.
	Anchor::Enum anchor;

	// Overlay position.
	Vector2i position;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Overlay );

//-----------------------------------//

} } // end namespaces