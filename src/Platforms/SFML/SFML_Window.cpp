/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "GameRuntime/API.h"
#include "SFML_Window.h"

#ifdef ENABLE_WINDOW_SFML

NAMESPACE_GAMERUNTIME_BEGIN

//-----------------------------------//

SFML_Window::SFML_Window(const WindowSettings& settings)
	: Window(settings)
	, cursorState(true)
	, flags(0)
{

}

//-----------------------------------//

SFML_Window::~SFML_Window()
{
	window.close();
}

//-----------------------------------//

bool SFML_Window::open()
{
	contextSettings.depthBits = settings.depthBits; 
	contextSettings.stencilBits = settings.stencilBits;
	contextSettings.antialiasingLevel = settings.antialiasLevel;
	
	videoMode = sf::VideoMode::getDesktopMode();

	if( settings.fullScreen )
	{
		flags |= sf::Style::Fullscreen;
	}
	else
	{
		videoMode.width = settings.width; 
		videoMode.height = settings.height;
		videoMode.bitsPerPixel = settings.bitsPerPixel;
		
		if( !videoMode.isValid() )
		{
			LogError( "Video mode not supported." );
			return false;
		}
		
		flags = sf::Style::Close;
	}
	
	createWindow();

	contextSettings = window.getSettings();
	
	assert( contextSettings.depthBits >= 0 && contextSettings.depthBits <= 32 );
	settings.depthBits = contextSettings.depthBits;
	
	assert( contextSettings.stencilBits >= 0 && contextSettings.stencilBits <= 32 );
	settings.stencilBits = contextSettings.stencilBits;
	
	assert( contextSettings.antialiasingLevel >= 0 && contextSettings.antialiasingLevel <= 32 );
	settings.antialiasLevel = contextSettings.antialiasingLevel;
	
	//window.EnableKeyRepeat( false );
	
	return true;
}

//-----------------------------------//

void SFML_Window::createWindow()
{
	if(settings.handle)
	{
		sf::WindowHandle handle = (sf::WindowHandle) settings.handle;
		window.create( handle, contextSettings );
	}
	else
	{
		window.create( videoMode, settings.title, flags, contextSettings );
	}

	context = AllocateThis(RenderContext);
	context->mainTarget = this;
}

//-----------------------------------//

void SFML_Window::update() 
{
	window.display();
}

//-----------------------------------//

void SFML_Window::show( bool visible ) 
{
	window.setVisible( visible );
}

//-----------------------------------//

void SFML_Window::makeCurrent()
{
	window.setActive();
}

//-----------------------------------//

bool SFML_Window::pumpEvents()
{
	sf::Event event;

	while( window.pollEvent(event) )
	{
	switch( event.type )
	{
	case sf::Event::Closed:
		handleWindowClose();
		return false;

	case sf::Event::Resized:
		processResize( event );
		break;

	case sf::Event::LostFocus:
		handleWindowFocus( true );
		break;

	case sf::Event::GainedFocus:
		handleWindowFocus( false );
		break;

	default:
		input.processSFMLEvent( event );
		break;
	} }

	return true;
}

//-----------------------------------//

void SFML_Window::setTitle(const String& title) 
{
	settings.title = title;
	createWindow();
	
	LogInfo( "Changing window title to '%s'", title.c_str() );
}

//-----------------------------------//

void SFML_Window::setCursorVisible(bool state)
{
	cursorState = state;
	window.setMouseCursorVisible(state);
}

//-----------------------------------//

bool SFML_Window::isCursorVisible() const
{
	return cursorState;
}

//-----------------------------------//

void SFML_Window::setCursorPosition( int x, int y )
{
	sf::Mouse::setPosition( sf::Vector2i(x, y), window );
}

//-----------------------------------//

Vector2i SFML_Window::getCursorPosition() const
{
	sf::Vector2i pos = sf::Mouse::getPosition();
	return Vector2i(pos.x, pos.y);
}

//-----------------------------------//

void SFML_Window::processResize(sf::Event event)
{
	settings.height = (uint16) event.size.height;
	settings.width = (uint16) event.size.width;

	handleWindowResize();
}

//-----------------------------------//

NAMESPACE_GAMERUNTIME_END

#endif