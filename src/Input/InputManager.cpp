/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Input/InputManager.h"
#include "Input/Device.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/Joystick.h"
#include "Window/Window.h"

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
	: window(nullptr)
{
	gs_InputManager = this;
}

//-----------------------------------//

InputManager::~InputManager()
{
	for( size_t i = 0; i < devices.size(); i++ )
	{
		InputDevice* device = devices[i];
		Deallocate(device);
	}
}

//-----------------------------------//

void InputManager::addDevice( InputDevice* device )
{
	if( !device )
	{
		LogWarn( "Tried to add an invalid input device" );
		return;
	}
	
	devices.push_back( device );

	LogInfo( "Registered a new input device: '%s'",
		EnumGetValueName(ReflectionGetType(InputDeviceType), device->getType()));
}

//-----------------------------------//

Keyboard* InputManager::getKeyboard() const
{
	for( size_t i = 0; i < devices.size(); i++ )
	{
		InputDevice* device = devices[i];
		if( device->getType() == InputDeviceType::Keyboard )
			return static_cast<Keyboard*>( device );
	}

	return nullptr;
}

//-----------------------------------//

Mouse* InputManager::getMouse() const
{
	for( size_t i = 0; i < devices.size(); i++ )
	{
		InputDevice* device = devices[i];
		if( device->getType() == InputDeviceType::Mouse )
			return static_cast<Mouse*>( device );
	}

	return nullptr;
}

//-----------------------------------//

void InputManager::processEvent( const InputEvent& event )
{
	for( size_t i = 0; i < devices.size(); i++ )
	{
		InputDevice* device = devices[i];
		device->processEvent( event );
	}
}

//-----------------------------------//

void InputManager::createDefaultDevices()
{
	addDevice( Allocate(gs_InputAllocator, Keyboard) );
	addDevice( Allocate(gs_InputAllocator, Mouse) );
}

//-----------------------------------//

void InputManager::setWindow(Window* window)
{
	this->window = window;

	if( window == nullptr )
	{
		Keyboard* keyboard = GetInputManager()->getKeyboard();
		if(keyboard) keyboard->resetKeys();
	}
}

//-----------------------------------//

NAMESPACE_ENGINE_END

