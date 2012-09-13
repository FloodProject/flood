/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

//---------------------------------------------------------------------//
// Core Includes
//---------------------------------------------------------------------//

#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Archive.h"
#include "Core/Event.h"
#include "Core/Serialization.h"
#include "Core/FileWatcher.h"

//---------------------------------------------------------------------//
// Math-related Includes
//---------------------------------------------------------------------//

#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Ray.h"
#include "Core/Math/BoundingBox.h"
#include "Core/Math/BoundingSphere.h"
#include "Core/Math/Noise.h"
#include "Core/Math/Hash.h"
#include "Core/Math/Helpers.h"

//---------------------------------------------------------------------//
// Audio-related Includes
//---------------------------------------------------------------------//

#include "Engine/Audio/Device.h"
#include "Engine/Audio/Buffer.h"
#include "Engine/Audio/Context.h"
#include "Engine/Audio/Source.h"

//---------------------------------------------------------------------//
// Resource-related Includes
//---------------------------------------------------------------------//

#include "Resources/Resource.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceLoader.h"

#include "Graphics/Resources/Image.h"
#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Sound.h"
#include "Engine/Resources/Font.h"
#include "Graphics/Resources/ShaderMaterial.h"

//---------------------------------------------------------------------//
// Scene-related Includes
//---------------------------------------------------------------------//

#include "Engine/Scene/Group.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Scene/Source.h"
#include "Engine/Scene/Listener.h"
#include "Engine/Scene/Geometry.h"
#include "Engine/Scene/Model.h"
#include "Engine/Scene/Grid.h"
#include "Engine/Scene/Light.h"
#include "Engine/Scene/Skydome.h"
#include "Engine/Scene/Water.h"
#include "Engine/Scene/Billboard.h"
#include "Engine/Scene/Projector.h"
#include "Engine/Scene/Particles.h"
#include "Engine/Scene/Tags.h"

//---------------------------------------------------------------------//
// Render-related Includes
//---------------------------------------------------------------------//

#include "Graphics/RenderDevice.h"
#include "Graphics/RenderView.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/RenderContext.h"
#include "Graphics/Buffer.h"
#include "Graphics/Texture.h"
#include "Graphics/Shader.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/RenderBatch.h"
#include "Graphics/TextureManager.h"
#include "Graphics/ShaderProgramManager.h"
#include "Graphics/FrameStatistics.h"

//---------------------------------------------------------------------//
// Animation-related Includes
//---------------------------------------------------------------------//

#include "Engine/Resources/Animation.h"
#include "Engine/Resources/Skeleton.h"
#include "Engine/Resources/Bone.h"
#include "Engine/Resources/Attachment.h"

//---------------------------------------------------------------------//
// Controller-related Includes
//---------------------------------------------------------------------//

#include "Engine/Controllers/FirstPersonController.h"
#include "Engine/Controllers/ThirdPersonController.h"
#include "Engine/Controllers/ScriptController.h"

//---------------------------------------------------------------------//
// Input-related Includes
//---------------------------------------------------------------------//

#include "Engine/Input/InputManager.h"
#include "Engine/Input/InputMap.h"
#include "Engine/Input/Keyboard.h"
#include "Engine/Input/Mouse.h"

//---------------------------------------------------------------------//
// GUI-related Includes
//---------------------------------------------------------------------//

#include "Engine/GUI/Overlay.h"
#include "Engine/GUI/Label.h"
#include "Engine/GUI/Window.h"
#include "Engine/GUI/Console.h"

//---------------------------------------------------------------------//
// Scripting-related Includes
//---------------------------------------------------------------------//

#include "Script/ScriptManager.h"
#include "Script/State.h"
#include "Script/Script.h"

//---------------------------------------------------------------------//
// Physics-related Includes
//---------------------------------------------------------------------//

#include "Engine/Physics/Physics.h"
#include "Engine/Physics/Body.h"
#include "Engine/Physics/BoxShape.h"
#include "Engine/Physics/CapsuleShape.h"
#include "Engine/Physics/MeshShape.h"
#include "Engine/Physics/CharacterController.h"

//---------------------------------------------------------------------//
// Terrain-related Includes
//---------------------------------------------------------------------//

#include "Engine/Terrain/Terrain.h"
#include "Engine/Terrain/Cell.h"

//---------------------------------------------------------------------//
// Paging-related Includes
//---------------------------------------------------------------------//

#include "Engine/Paging/PageManager.h"
#include "Engine/Paging/Page.h"

//---------------------------------------------------------------------//
// General Includes
//---------------------------------------------------------------------//

#include "Engine/Engine.h"
