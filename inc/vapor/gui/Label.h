/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/gui/Overlay.h"

FWD_DECL_NS_TYPEDEF_INT(resources, Font)
FWD_DECL_NS_TYPEDEF_INT(render, Material)
FWD_DECL_NS_TYPEDEF_SHARED(scene, Geometry)

namespace vapor { namespace gui {

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

class VAPOR_API Label : public gui::Overlay
{
public:

	Label( const std::string& text, resources::FontPtr font, render::MaterialPtr mat );
	Label( const std::string& text, const std::string& font );

	// Gets the current text of this label.
	IMPLEMENT_GETTER(Text, const std::string&, text)

	// Sets the current text of this label.
	void setText( const std::string& text );

	// Updates the geometry to match the new text if needed.
	virtual void update( double delta );

	// Returns this component name identification.
	IMPLEMENT_GETTER(Type, const std::string&, type)

protected:

	// Initializes the label.
	void init();

	// Initializes the label once the font is loaded.
	void setupState();

	// Contains the text of the label.
	std::string text;
	
	// Holds the font texture we are gonna use for rendering the glyphs.
	resources::FontPtr font;
	
	// Font geometry.
	render::RenderablePtr renderable;

	// Font material.
	render::MaterialPtr material;
	
	// Used to track if a label needs updating.
	bool isDirty;

	bool setupDone;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Label );

//-----------------------------------//

} } // end namespaces