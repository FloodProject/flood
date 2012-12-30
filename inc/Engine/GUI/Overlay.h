/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Scene/Geometry.h"
#include "Core/Math/Vector.h"

NAMESPACE_ENGINE_BEGIN

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

API_ENGINE REFLECT_DECLARE_ENUM(PositionMode)

struct PositionMode
{
	enum Enum
	{
		Relative,
		Absolute
	};
};

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

API_ENGINE REFLECT_DECLARE_ENUM(AnchorMode)

struct AnchorMode
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
};

//-----------------------------------//

/**
 * An overlay is a screen-space element that can be used for rendering
 * all kinds of visual elements on the screen, for example GUI widgets.
 * Each overlay can be positioned in two different ways: Relative-mode
 * positioning and Absolute-mode positioning. For mode information on
 * these different positioning modes, please check out the docs above.
 */

API_ENGINE REFLECT_DECLARE_CLASS(Overlay)

class API_ENGINE Overlay : public Geometry
{
	REFLECT_DECLARE_OBJECT(Overlay)

public:

	Overlay();

	// Gets/sets the current positioning mode.
	ACCESSOR(PositionMode, PositionMode::Enum, positioning) 

	// Gets/sets the current anchor settings.
	ACCESSOR(AnchorMode, AnchorMode::Enum, anchor)

	// Gets/sets the offset.
	ACCESSOR(Offset, const Vector3&, offset)

	// Gets/sets the size.
	ACCESSOR(Size, const Vector3&, size)

	// Gets/sets the background color.
	ACCESSOR(BackgroundColor, Color, backgroundColor)

	// Gets/sets the border width.
	ACCESSOR(BorderWidth, int, borderWidth)

	// Gets/sets the border color.
	ACCESSOR(BorderColor, Color, borderColor)

	// Sets the offset of the overlay.
	void setOffset( int x, int y );

	// Sets the opacity of the overlay.
	void setOpacity( float opacity );

	// Updates the overlay if needed.
	virtual void update( float delta ) OVERRIDE;

	// Layouts the overlay.
	void layout(const Vector2i& targetSize);

protected:

	// Creates the overlay geometry.
	void createGeometry();

	// Rebuilds the overlay geometry.
	virtual void rebuildGeometry();

	// Gets called before rendering.
	void onPreRender(RenderView* view, const RenderState&);

	// Positioning mode used.
	PositionMode::Enum positioning;

	// Anchoring mode used.
	AnchorMode::Enum anchor;

	// Overlay offset.
	Vector3 offset;

	// Overlay size.
	Vector3 size;

	// Overlay layout position.
	Vector2i position;

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

	// Overlay material.
	MaterialHandle material;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Overlay );

//-----------------------------------//

NAMESPACE_ENGINE_END