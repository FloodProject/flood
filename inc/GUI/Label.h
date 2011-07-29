/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "GUI/Overlay.h"

#include "Resources/Font.h"
#include "Resources/Material.h"

NAMESPACE_ENGINE_BEGIN

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

REFLECT_DECLARE_CLASS(Label)

class API_ENGINE Label : public Overlay
{
	REFLECT_DECLARE_OBJECT(Label)

public:

	Label();
	Label( const String& text, const FontHandle&, const MaterialHandle& );
	Label( const String& text, const String& font );

	// Gets the current text of this label.
	GETTER(Text, const String&, text)

	// Sets the current text of this label.
	void setText( const String& text );

	// Updates the geometry to match the new text if needed.
	virtual void update( float delta );

protected:

	// Initializes the label.
	void init();

	// Rebuilds the overlay geometry.
	virtual void rebuildGeometry();

	// Initializes the label once the font is loaded.
	void setupState();

	// Contains the text of the label.
	String text;
	
	// Holds the font texture we are gonna use for rendering the glyphs.
	FontHandle font;
	
	// Used to track if a label needs updating.
	bool isDirty;

	bool setupDone;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Label );

//-----------------------------------//

NAMESPACE_ENGINE_END