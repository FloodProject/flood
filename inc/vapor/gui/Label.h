/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/gui/Overlay.h"

FWD_DECL_INTRUSIVE(Font)
FWD_DECL_INTRUSIVE(Material)
FWD_DECL_SHARED(Geometry)

namespace vapor {

//-----------------------------------//

/**
 * Text label that can be rendered on the screen with a chosen material.
 * It's positioned in screen-space instead of in world-space so you give
 * naturally it's positioning is resolution-dependent and you should take
 * the resolution into account when doing so. For ease of use, you can also
 * align it to a determined "anchor point" and give relative offsets from
 * that point, and this it will be automatically placed even if the user
 * changes the resolution.
 */

class VAPOR_API Label : public Overlay
{
	DECLARE_CLASS_()

public:

	Label( const std::string& text, FontPtr font, MaterialPtr mat );
	Label( const std::string& text, const std::string& font );

	// Gets the current text of this label.
	GETTER(Text, const std::string&, text)

	// Sets the current text of this label.
	void setText( const std::string& text );

	// Updates the geometry to match the new text if needed.
	virtual void update( double delta );

protected:

	// Initializes the label.
	void init();

	// Rebuilds the label geometry.
	void rebuildGeometry();

	// Initializes the label once the font is loaded.
	void setupState();

	// Contains the text of the label.
	std::string text;
	
	// Holds the font texture we are gonna use for rendering the glyphs.
	FontPtr font;
	
	// Font geometry.
	RenderablePtr renderable;

	// Font material.
	MaterialPtr material;
	
	// Used to track if a label needs updating.
	bool isDirty;

	bool setupDone;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Label );

//-----------------------------------//

} // end namespace