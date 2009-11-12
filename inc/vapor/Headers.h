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
#include <vapor/resources/Font.h>

//-------------------------------------------------------------------------//
// Scene-related Includes
//-------------------------------------------------------------------------//

#include <vapor/scene/Group.h>
#include <vapor/scene/Scene.h>
#include <vapor/scene/Camera.h>
#include <vapor/scene/FirstPersonCamera.h>
#include <vapor/scene/Sound.h>
#include <vapor/scene/Listener.h>
#include <vapor/scene/Geometry.h>
#include "vapor/scene/Grid.h"

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
#include "vapor/render/ProgramManager.h"

//-------------------------------------------------------------------------//
// Input-related Includes
//-------------------------------------------------------------------------//

#include <vapor/input/InputManager.h>
#include <vapor/input/InputMap.h>
#include <vapor/input/Keyboard.h>
#include <vapor/input/Mouse.h>

//-------------------------------------------------------------------------//
// GUI-related Includes
//-------------------------------------------------------------------------//

#include <vapor/gui/Overlay.h>
#include <vapor/gui/Label.h>
#include <vapor/gui/Window.h>
#include <vapor/gui/Console.h>

//-------------------------------------------------------------------------//
// Scripting-related Includes
//-------------------------------------------------------------------------//

#include <vapor/script/State.h>
#include <vapor/script/Script.h>

//-------------------------------------------------------------------------//
// General Includes
//-------------------------------------------------------------------------//

#include <vapor/Engine.h>

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
using namespace vapor::gui;
using namespace vapor::script;

#endif