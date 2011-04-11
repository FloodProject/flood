/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

//---------------------------------------------------------------------//
// Core Includes
//---------------------------------------------------------------------//

#include <Core/Log.h>
#include "Core/Memory.h"
#include <Core/Archive.h>
#include <Core/Event.h>
#include <Core/Serialization.h>
#include <Core/FileWatcher.h>

//---------------------------------------------------------------------//
// Math-related Includes
//---------------------------------------------------------------------//

#include <Math/Vector.h>
#include <Math/Color.h>
#include <Math/Quaternion.h>
#include <Math/Ray.h>
#include <Math/BoundingBox.h>
#include <Math/BoundingSphere.h>
#include <Math/Noise.h>
#include <Math/Hash.h>
#include <Math/Helpers.h>

//---------------------------------------------------------------------//
// Audio-related Includes
//---------------------------------------------------------------------//

#include <Audio/Device.h>
#include <Audio/Buffer.h>
#include <Audio/Context.h>
#include <Audio/Source.h>

//---------------------------------------------------------------------//
// Resource-related Includes
//---------------------------------------------------------------------//

#include "Resources/Resource.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceLoader.h"
#include <Resources/Image.h>
#include <Resources/Mesh.h>
#include <Resources/Sound.h>
#include <Resources/Font.h>

//---------------------------------------------------------------------//
// Scene-related Includes
//---------------------------------------------------------------------//

#include <Scene/Group.h>
#include <Scene/Scene.h>
#include <Scene/Camera.h>
#include <Scene/Source.h>
#include <Scene/Listener.h>
#include <Scene/Geometry.h>
#include <Scene/Model.h>
#include <Scene/Grid.h>
#include <Scene/Light.h>
#include <Scene/Skydome.h>
#include <Scene/Water.h>
#include <Scene/Billboard.h>
#include <Scene/Projector.h>
#include <Scene/Particles.h>
#include <Scene/Tags.h>

//---------------------------------------------------------------------//
// Render-related Includes
//---------------------------------------------------------------------//

#include <Render/Device.h>
#include <Render/View.h>
#include <Render/Target.h>
#include <Render/VertexBuffer.h>
#include <Render/Texture.h>
#include <Render/Program.h>
#include <Render/Renderable.h>
#include <Resources/Shader.h>
#include <Render/Quad.h>
#include <Render/TextureManager.h>
#include <Render/ProgramManager.h>
#include <Render/FBO.h>
#include <Render/FrameStatistics.h>
#include <Render/DebugGeometry.h>

//---------------------------------------------------------------------//
// Animation-related Includes
//---------------------------------------------------------------------//

#include <Resources/Animation.h>
#include <Resources/Skeleton.h>
#include <Resources/Bone.h>
#include <Resources/Attachment.h>

//---------------------------------------------------------------------//
// Controller-related Includes
//---------------------------------------------------------------------//

#include <controllers/FirstPersonController.h>
#include <controllers/ThirdPersonController.h>
#include <controllers/ScriptController.h>

//---------------------------------------------------------------------//
// Input-related Includes
//---------------------------------------------------------------------//

#include <Input/InputManager.h>
#include <Input/InputMap.h>
#include <Input/Keyboard.h>
#include <Input/Mouse.h>

//---------------------------------------------------------------------//
// GUI-related Includes
//---------------------------------------------------------------------//

#include <gui/Overlay.h>
#include <gui/Label.h>
#include <gui/Window.h>
#include <gui/Console.h>

//---------------------------------------------------------------------//
// Scripting-related Includes
//---------------------------------------------------------------------//

#include <script/ScriptManager.h>
#include <script/State.h>
#include <script/Script.h>

//---------------------------------------------------------------------//
// Physics-related Includes
//---------------------------------------------------------------------//

#include <physics/Physics.h>
#include <physics/Body.h>
#include <physics/BoxShape.h>
#include <physics/CapsuleShape.h>
#include <physics/MeshShape.h>
#include <physics/CharacterController.h>

//---------------------------------------------------------------------//
// Terrain-related Includes
//---------------------------------------------------------------------//

#include <terrain/Terrain.h>
#include <terrain/Cell.h>

//---------------------------------------------------------------------//
// Paging-related Includes
//---------------------------------------------------------------------//

#include <paging/PageManager.h>
#include <paging/Page.h>

//---------------------------------------------------------------------//
// General Includes
//-----------------------------------------------3----------------------//

#include <Engine.h>
