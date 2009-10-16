/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/resources/Font.h"
#include "vapor/render/Material.h"
#include "vapor/scene/Geometry.h"

namespace vapor {
	namespace gui {

//-----------------------------------//

/**
 * Anchor points for positioning the label on the screen. When you specify 
 * an alignment, the positioning of the label will be relative to the point
 * you specified. Here is some awesome ASCII art exemplifying it all:
 *		 ______________
 *		|              |
 *		| TL   TC   TR |
 *		|              |
 *		| L    C     R |
 *		|              |
 *		| BL   BC   BR |
 *		|______________|
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
		Unanchored
	};
}

//-----------------------------------//

/**
 * Text label that can be rendered on the screen with a determined material.
 * It's positioned in screen-space instead of in world-space so you give
 * naturally it's positioning is resolution-dependent and you should take
 * the resolution into account when doing so. For ease of use, you can also
 * align it to a determined "anchor point" and give relative offsets from
 * that point, and this it will be automatically placed even if the user
 * changes the resolution.
 */

class Label : public scene::Geometry
{
public:

	Label( const std::string& text, resources::FontPtr font,
		render::MaterialPtr mat,
		Anchor::Enum anchor = Anchor::Unanchored );

	//Label( const std::string& text, std::string font, 
		//Anchor::Enum anchor = Anchor::Unanchored );

	virtual ~Label();

	// Gets the current text of this label.
	const std::string& getText() const;

	// Sets the current text of this label.
	void setText( std::string text );

	// Gets the current anchor settings.
	Anchor::Enum getAnchor() const;

	// Sets the current anchor settings.
	void setAnchor( Anchor::Enum anchor );

	// Gets the positioning of the label.
	std::pair<int,int> getPosition() const;
	
	// Sets the positioning of the label.
	void setPosition( int x, int y );

	// Updates the geometry to match the new text if needed.
	virtual void update( double delta );

	// Serializes this node to a stream.
	virtual const std::string save(int indent = 0) { return ""; }

	// Returns this node name identification.
	virtual const std::string name() const { return "Label"; }

private:

	int x, y;
	std::string text;
	Anchor::Enum anchor;
	resources::FontPtr font;
	render::RenderablePtr renderable;
	bool isDirty;
};

//-----------------------------------//

typedef tr1::shared_ptr< gui::Label > LabelPtr;

//-----------------------------------//

} } // end namespaces