/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/Resources/ShaderMaterial.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ShaderMaterial, Resource)
REFLECT_CLASS_END()

//-----------------------------------//

ShaderMaterial::ShaderMaterial()
{
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END