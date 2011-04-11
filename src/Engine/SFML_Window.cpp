/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/SFML_Window.h"

#ifdef VAPOR_WINDOWING_SFML

namespace vapor {

//-----------------------------------//

SFML_Window::SFML_Window(const WindowSettings& settings)
	: Window(settings)
	, cursorState(true)
	, flags(0)
{
	if( !open() )
	{
		LogError( "Could not create SFML render window" );
	}
}

//-----------------------------------//

SFML_Window::~SFML_Window()
{
	window.Close();
}

//-----------------------------------//

bool SFML_Window::open()
{
	windowSettings.DepthBits = settings.depthBits; 
	windowSettings.StencilBits = settings.stencilBits;
	windowSettings.AntialiasingLevel = settings.antialiasLevel;
	
	if( settings.fullScreen )
	{
		videoMode = sf::VideoMode::GetMode(0);
		flags |= sf::Style::Fullscreen;
	}
	else
	{
		videoMode.Width = settings.width; 
		videoMode.Height = settings.height;
		videoMode.BitsPerPixel = settings.bitsPerPixel;
		
		if( !videoMode.IsValid() )
		{
			LogError( "Video mode not supportted." );
			return false;
		}
		
		flags |= sf::Style::Resize | sf::Style::Close;
	}
	
	createWindow();

	windowSettings = window.GetSettings();
	
	assert( windowSettings.DepthBits >= 0 && windowSettings.DepthBits <= 32 );
	settings.depthBits = windowSettings.DepthBits;
	
	assert( windowSettings.StencilBits >= 0 && windowSettings.StencilBits <= 32 );
	settings.stencilBits = windowSettings.StencilBits;
	
	assert( windowSettings.AntialiasingLevel >= 0 && windowSettings.AntialiasingLevel <= 32 );
	settings.antialiasLevel = windowSettings.AntialiasingLevel;
	
	window.EnableKeyRepeat( false );
	
	return true;
}

//-----------------------------------//

void SFML_Window::createWindow()
{
	if(settings.handle)
	{
		sf::WindowHandle handle( settings.handle );
		window.Create( handle, windowSettings );
	}
	else
	{		
		window.Create( videoMode, settings.title, flags, windowSettings );
	}
}

//-----------------------------------//

void SFML_Window::update() 
{
	window.Display();
}

//-----------------------------------//

void SFML_Window::show( bool hide ) 
{
	window.Show( hide );
}

//-----------------------------------//

void SFML_Window::makeCurrent()
{
	window.SetActive(); 
}

//-----------------------------------//

bool SFML_Window::pumpEvents()
{
	sf::Event event;

	while( window.GetEvent( event ) )
	{
		switch( event.Type )
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
			
		case sf::Event::TextEntered:
			
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
			
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:

		case sf::Event::MouseEntered:
		case sf::Event::MouseLeft:
		case sf::Event::MouseWheelMoved:
			
		case sf::Event::JoyButtonPressed:
		case sf::Event::JoyButtonReleased:
		case sf::Event::JoyMoved:
		{
			inputManager.processSFMLEvent( event );
			break;
		}

		case sf::Event::MouseMoved:
		{
			inputManager.processSFMLEvent( event );
			break;
		}
		}
	}

	return true;
}

//-----------------------------------//

void SFML_Window::setTitle(const std::string& title) 
{
	settings.title = title;
	createWindow();
	
	LogInfo( "Changing window title to '%s'", title.c_str() );
}

//-----------------------------------//

void SFML_Window::setCursorVisible(bool state)
{
	cursorState = state;
	window.ShowMouseCursor(state);
}

//-----------------------------------//

bool SFML_Window::isCursorVisible() const
{
	return cursorState;
}

//-----------------------------------//

void SFML_Window::setCursorPosition( int x, int y )
{
	window.SetCursorPosition( x, y );
}

//-----------------------------------//

Vector2 SFML_Window::getCursorPosition() const
{
	int x = window.GetInput().GetMouseX();
	int y = window.GetInput().GetMouseY();

	return Vector2(x, y);
}

//-----------------------------------//

void SFML_Window::processResize(sf::Event event)
{
	settings.height = (uint16) event.Size.Height;
	settings.width = (uint16) event.Size.Width;

	handleWindowResize();
}

//-----------------------------------//

} // end namespace

#endif