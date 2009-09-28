/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

//-------------------------------------------------------------------------//
// Audio-related Includes
//-------------------------------------------------------------------------//

#include <vapor/audio/Device.h>

//-------------------------------------------------------------------------//
// Resource-related Includes
//-------------------------------------------------------------------------//

#include <vapor/resources/Image.h>
#include <vapor/resources/MS3D.h>
#include <vapor/resources/Sound.h>
#include <vapor/resources/Shader.h>

//-------------------------------------------------------------------------//
// Scene-related Includes
//-------------------------------------------------------------------------//

#include <vapor/scene/Camera.h>
#include <vapor/scene/FirstPersonCamera.h>
#include <vapor/scene/Sound.h>
#include <vapor/scene/Listener.h>
#include <vapor/scene/Geometry.h>

//-------------------------------------------------------------------------//
// Render-related Includes
//-------------------------------------------------------------------------//

#include <vapor/render/VertexBuffer.h>
#include <vapor/render/Texture.h>
#include <vapor/render/Program.h>
#include <vapor/render/Renderable.h>
#include <vapor/render/GLSL_Program.h>
#include <vapor/render/GLSL_Shader.h>
#include <vapor/render/Quad.h>
#include "vapor/render/TextureManager.h"

//-------------------------------------------------------------------------//
// Input-related Includes
//-------------------------------------------------------------------------//

#include <vapor/input/InputManager.h>
#include <vapor/input/InputMap.h>
#include <vapor/input/Keyboard.h>
#include <vapor/input/Mouse.h>

//-------------------------------------------------------------------------//
// Namespace declarations
//-------------------------------------------------------------------------//

#ifdef VAPOR_USE_NAMESPACES

using namespace vapor;
using namespace vapor::vfs;
using namespace vapor::log;
using namespace vapor::math;
using namespace vapor::scene;
using namespace vapor::audio;
using namespace vapor::render;
using namespace vapor::resources;
using namespace vapor::input;

#endif