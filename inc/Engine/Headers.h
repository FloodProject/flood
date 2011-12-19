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
#include <Resources/Shader.h>

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

#include <Graphics/RenderDevice.h>
#include <Graphics/RenderView.h>
#include <Graphics/RenderTarget.h>
#include "Graphics/RenderContext.h"
#include <Graphics/Buffer.h>
#include <Graphics/Texture.h>
#include <Graphics/Program.h>
#include <Graphics/VertexBuffer.h>
#include <Graphics/RenderBatch.h>
#include <Graphics/TextureManager.h>
#include <Graphics/ProgramManager.h>
#include <Graphics/FrameStatistics.h>

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

#include <Controllers/FirstPersonController.h>
#include <Controllers/ThirdPersonController.h>
#include <Controllers/ScriptController.h>

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

#include <GUI/Overlay.h>
#include <GUI/Label.h>
#include <GUI/Window.h>
#include <GUI/Console.h>

//---------------------------------------------------------------------//
// Scripting-related Includes
//---------------------------------------------------------------------//

#include <Script/ScriptManager.h>
#include <Script/State.h>
#include <Script/Script.h>

//---------------------------------------------------------------------//
// Physics-related Includes
//---------------------------------------------------------------------//

#include <Physics/Physics.h>
#include <Physics/Body.h>
#include <Physics/BoxShape.h>
#include <Physics/CapsuleShape.h>
#include <Physics/MeshShape.h>
#include <Physics/CharacterController.h>

//---------------------------------------------------------------------//
// Terrain-related Includes
//---------------------------------------------------------------------//

#include <Terrain/Terrain.h>
#include <Terrain/Cell.h>

//---------------------------------------------------------------------//
// Paging-related Includes
//---------------------------------------------------------------------//

#include <Paging/PageManager.h>
#include <Paging/Page.h>

//---------------------------------------------------------------------//
// General Includes
//---------------------------------------------------------------------//

#include <Engine/Engine.h>
