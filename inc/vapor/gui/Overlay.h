/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/scene/Geometry.h"

namespace vapor {
	namespace gui {

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
 * these different positioning modes, please check out the docs on the
 * Positioning enum.
 */

class VAPOR_API Overlay : public virtual scene::Geometry
{
public:

	Overlay();
	virtual ~Overlay();

	// Gets the current positioning mode.
	Positioning::Enum getPositioning() const;

	// Sets the current  positioning mode.
	void setPositioning( Positioning::Enum positioning );

	// Gets the current anchor settings.
	Anchor::Enum getAnchor() const;

	// Sets the current anchor settings.
	void setAnchor( Anchor::Enum anchor );

	// Gets the positioning of the label.
	//std::pair<int,int> getPosition() const;

	// Sets the position of the overlay.
	//void setPosition( int x, int y );

	// Updates the overlay if needed.
	virtual void update( float delta );

	// Serializes this node to a stream.
	virtual const std::string save( int indent = 0 );

	// Returns this component name identification.
	virtual const std::string& getType() const;

protected:

	// Positioning mode used.
	Positioning::Enum positioning;

	// Anchoring mode used.
	Anchor::Enum anchor;

	// Overlay position.
	int x, y;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Overlay );

//-----------------------------------//

} } // end namespaces