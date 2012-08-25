/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Resources/Font.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Font, Resource)
REFLECT_CLASS_END()

//-----------------------------------//

Font::Font()
	: size(0)
{ }

//-----------------------------------//

NAMESPACE_RESOURCES_END