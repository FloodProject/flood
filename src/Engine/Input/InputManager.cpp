/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Input/InputManager.h"
#include "Engine/Input/Device.h"
#include "Engine/Input/Keyboard.h"
#include "Engine/Input/Mouse.h"
#include "Engine/Input/Joystick.h"
#include "Engine/Window/Window.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

static Allocator* gs_InputAllocator = nullptr;

static InputManager* gs_InputManager = nullptr;
InputManager* GetInputManager() { return gs_InputManager; }

void InputInitialize()
{
	gs_InputAllocator = AllocatorCreateHeap( AllocatorGetHeap() );
	AllocatorSetGroup(gs_InputAllocator, "Input");
}

void InputDeinitialize()
{
	AllocatorDestroy(gs_InputAllocator);
}

//-----------------------------------//

InputManager::InputManager()
{
	gs_InputManager = this;
}

//-----------------------------------//

InputManager::~InputManager()
{
}

//-----------------------------------//

NAMESPACE_ENGINE_END

