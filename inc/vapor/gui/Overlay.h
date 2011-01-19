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

namespace vapor {

//-----------------------------------//

/**
 * Each overlay can be positioned in two different ways: relative-mode
 * positioning and absolute-mode positioning. In relative-mode you can
 * set an anchor (see the Anchor enum documentation for detailed info
 * about how it works and the different modes you can choose) and the
 * overlay position will be automatically adjusted by the engine even
 * if the resolution changes. So imagine you anchor it to the top left
 * and set a 5-pixel x-offset from it, then it will always stay there.
 */ 

namespace PositionMode
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

class VAPOR_API Overlay : public Geometry
{
	DECLARE_CLASS_()

public:

	Overlay();

	// Gets/sets the current positioning mode.
	ACESSOR(PositionMode, PositionMode::Enum, positioning) 

	// Gets/sets the current anchor settings.
	ACESSOR(Anchor, Anchor::Enum, anchor)

	// Gets/sets the position.
	ACESSOR(Position, const Vector2i&, position)

	// Gets/sets the size.
	ACESSOR(Size, const Vector2i&, size)

	// Gets/sets the background color.
	ACESSOR(BackgroundColor, Color, backgroundColor)

	// Gets/sets the border width.
	ACESSOR(BorderWidth, int, borderWidth)

	// Gets/sets the border color.
	ACESSOR(BorderColor, Color, borderColor)

	// Sets the position of the overlay.
	void setPosition( int x, int y );

	// Sets the opacity of the overlay.
	void setOpacity( float opacity );

	// Updates the overlay if needed.
	virtual void update( double delta );

protected:

	// Creates the overlay geometry.
	void createGeometry();

	// Rebuilds the overlay geometry.
	virtual void rebuildGeometry();

	// Rebuilds the overlay geometry.
	void rebuildBorderGeometry();

	// PositionMode mode used.
	PositionMode::Enum positioning;

	// Anchoring mode used.
	Anchor::Enum anchor;

	// Overlay position.
	Vector2i position;

	// Overlay size.
	Vector2i size;

	// Background color.
	Color backgroundColor;

	// Border width.
	int borderWidth;

	// Border color.
	Color borderColor;

	// Opacity of the overlay.
	float opacity;

	// Overlay geometry.
	RenderablePtr renderable;

	// Border geometry.
	RenderablePtr borderRenderable;

	// Overlay material.
	MaterialPtr material;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Overlay );

//-----------------------------------//

} // end namespace