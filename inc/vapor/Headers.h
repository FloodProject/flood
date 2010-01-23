/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

//-------------------------------------------------------------------------//
// Audio-related Includes
//-------------------------------------------------------------------------//

#include <vapor/audio/Device.h>
#include <vapor/audio/Buffer.h>
#include <vapor/audio/Context.h>
#include <vapor/audio/Source.h>

//-------------------------------------------------------------------------//
// Math-related Includes
//-------------------------------------------------------------------------//

#include <vapor/math/Math.h>
#include <vapor/math/Vector3.h>
#include <vapor/math/AABB.h>
#include <vapor/math/Ray.h>

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
#include "vapor/scene/Light.h"

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
// VFS-related Includes
//-------------------------------------------------------------------------//

#include <vapor/vfs/VFS.h>
#include <vapor/vfs/File.h>
#include <vapor/vfs/Watcher.h>

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
// Physics-related Includes
//-------------------------------------------------------------------------//

#include <vapor/physics/Physics.h>
#include <vapor/scene/Body.h>

//-------------------------------------------------------------------------//
// Terrain-related Includes
//-------------------------------------------------------------------------//

#include <vapor/terrain/Terrain.h>
#include <vapor/terrain/Cell.h>

//-------------------------------------------------------------------------//
// General Includes
//-------------------------------------------------------------------------//

#include <vapor/Engine.h>
#include <vapor/Log.h>
#include <vapor/Profiler.h>

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