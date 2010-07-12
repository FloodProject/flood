/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/SFML_Window.h"

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
		error( "render", "Could not create SFML render window" );
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
	windowSettings.DepthBits = settings.getDepthBits(); 
	windowSettings.StencilBits = settings.getStencilBits();
	windowSettings.AntialiasingLevel = settings.getAntialiasing();
	
	if( settings.isFullscreen() )
	{
		videoMode = sf::VideoMode::GetMode(0);
		flags |= sf::Style::Fullscreen;
	}
	else
	{
		videoMode.Width = settings.getWidth(); 
		videoMode.Height = settings.getHeight();
		videoMode.BitsPerPixel = settings.getBpp();
		
		if( !videoMode.IsValid() )
		{
			error( "window::sfml", "Video mode not supportted." );
			return false;
		}
		
		flags |= sf::Style::Resize | sf::Style::Close;
	}
	
	createWindow();

	windowSettings = window.GetSettings();
	
	assert( windowSettings.DepthBits >= 0 && windowSettings.DepthBits <= 32 );
	settings.setDepthBits( windowSettings.DepthBits );
	
	assert( windowSettings.StencilBits >= 0 && windowSettings.StencilBits <= 32 );
	settings.setStencilBits( windowSettings.StencilBits );
	
	assert( windowSettings.AntialiasingLevel >= 0 && windowSettings.AntialiasingLevel <= 32 );
	settings.setAntialiasing( windowSettings.AntialiasingLevel );
	
	window.EnableKeyRepeat( false );
	
	return true;
}

//-----------------------------------//

void SFML_Window::createWindow()
{
	if(settings.getCustomHandle())
	{
		sf::WindowHandle handle( settings.getCustomHandle() );
		window.Create( handle, windowSettings );
	}
	else
	{		
		window.Create( videoMode, settings.getTitle(), flags, windowSettings );
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
	settings.setTitle(title);
	createWindow();
	
	info( "window::sfml", "Changing window title to '%s'", title.c_str() );
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

Vector2i SFML_Window::getCursorPosition() const
{
	int x = window.GetInput().GetMouseX();
	int y = window.GetInput().GetMouseY();

	return Vector2i(x, y);
}

//-----------------------------------//

void SFML_Window::processResize(sf::Event event)
{
	settings.setHeight( (ushort) event.Size.Height );
	settings.setWidth( (ushort) event.Size.Width );

	handleWindowResize();
}

//-----------------------------------//

} // end namespace

#endif