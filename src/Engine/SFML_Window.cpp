/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/SFML_Window.h"

#ifdef VAPOR_WINDOWING_SFML

using namespace vapor::log;

namespace vapor {
	namespace render {

//-----------------------------------//

SFML_Window::SFML_Window(const Settings& settings)
	: Window(settings), flags(0)
{
	if( !open() )
	{
		warn( "render", "Could not create SFML render window" );
		Log::MessageDialog( "Could not create SFML render window",
			LogLevel::Error );
		exit( 1 );
	}
}

//-----------------------------------//

SFML_Window::~SFML_Window()
{

}

//-----------------------------------//

bool SFML_Window::open()
{
	sfmlSettings.DepthBits = settings.getDepthBits(); 
	sfmlSettings.StencilBits = settings.getStencilBits();
	sfmlSettings.AntialiasingLevel = settings.getAALevel();
	
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
	settings.setAALevel( sfmlSettings.AntialiasingLevel );
	
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
			case sf::Event::MouseMoved:
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

void SFML_Window::setCursor(bool state)
{
	window.ShowMouseCursor(state);
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