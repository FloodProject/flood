/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/SFML_Window.h"

#ifdef VAPOR_WINDOWING_SFML

using namespace vapor::log;

namespace vapor { namespace render {

//-----------------------------------//

SFML_Window::SFML_Window(const WindowSettings& settings)
	: Window( settings ), flags( 0 ), cursorState( true )
{
	if( !open() )
	{
		warn( "render", "Could not create SFML render window" );
		Log::MessageDialog( "Could not create SFML render window",
			LogLevel::Error );
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
	sfmlSettings.DepthBits = settings.getDepthBits(); 
	sfmlSettings.StencilBits = settings.getStencilBits();
	sfmlSettings.AntialiasingLevel = settings.getAntiAliasing();
	
	if( settings.isFullscreen() )
	{
		vMode = sf::VideoMode::GetMode(0);
		flags |= sf::Style::Fullscreen;
	}
	else
	{
		vMode.Width = settings.getWidth(); 
		vMode.Height = settings.getHeight();
		vMode.BitsPerPixel = settings.getBpp();
		
		if( !vMode.IsValid() )
		{
			error( "render::window::sfml", "Video mode not supportted." );
			return false;
		}
		
		flags |= sf::Style::Resize | sf::Style::Close;
	}
	
	createWindow();

	sfmlSettings = window.GetSettings();
	settings.setDepthBits( sfmlSettings.DepthBits );
	settings.setStencilBits( sfmlSettings.StencilBits );
	settings.setAntiAliasing( sfmlSettings.AntialiasingLevel );
	
	window.EnableKeyRepeat( false );
	
	return true;	
}

//-----------------------------------//

void SFML_Window::createWindow()
{
	if(settings.getCustomHandle())
	{
		window.Create( sf::WindowHandle( settings.getCustomHandle() ),
			sfmlSettings );	
	}
	else
	{		
		window.Create( vMode, settings.getTitle(), flags, sfmlSettings );
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
				//still to figure out 
				break;	
			
			case sf::Event::GainedFocus:
				//still to figure out
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
				getInputManager().processSFMLEvent( event );
				break;
			}

			// If the mouse cursor is not visible, set the mouse cursor
			// position back to the center, if not the mouse gets off
			// the screen and won't be usable for i.e., camera rotations.
			case sf::Event::MouseMoved:
			{
				getInputManager().processSFMLEvent( event );
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
	
	info( "render::window::sfml", "Changing window title to '%s'", title.c_str() );
}

//-----------------------------------//

void SFML_Window::setCursorState(bool state)
{
	cursorState = state;
	window.ShowMouseCursor(state);
}

//-----------------------------------//

bool SFML_Window::getCursorState() const
{
	return cursorState;
}

//-----------------------------------//

void SFML_Window::setCursorPosition( int x, int y )
{
	window.SetCursorPosition( x, y );
}

//-----------------------------------//

void SFML_Window::processResize(sf::Event event)
{
	settings.setHeight( event.Size.Height );
	settings.setWidth( event.Size.Width );

	handleWindowResize();
}

//-----------------------------------//

} } // end namespaces

#endif