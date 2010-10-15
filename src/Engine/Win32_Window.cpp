/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Win32_Window.h"

#ifdef VAPOR_WINDOWING_WIN32

namespace vapor {

//-----------------------------------//

Win32Window::Win32Window(Settings& settings)
	: Window(settings)
	, hInstance(nullptr)
	, hDC(nullptr)
	, hWnd(nullptr)
	, hRC(nullptr)
{
	hInstance = GetModuleHandle(nullptr);

	if ( !registerClass() || !createWindow() ) 
	{
		Log::error("Could not open a Win32 window");
		//exit(1);
	}
}

//-----------------------------------//

Win32Window::~Win32Window()
{
	// Unregister class
	// Destroy context
	// Destroy window
}

//-----------------------------------//

bool Win32Window::registerClass()
{	
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC ;
	wc.lpfnWndProc   = (WNDPROC) WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon( nullptr, IDI_WINLOGO );
	wc.hCursor       = LoadCursor( nullptr, IDC_ARROW );
	wc.hbrBackground = nullptr;
	wc.lpszMenuName  = nullptr;
	wc.lpszClassName = TEXT("vapor_Window");

	if (!RegisterClass(&wc)) 
	{
		Log::error( "RegisterClass() failed: %s", getErrorMessage().c_str() );
		return false;
	}

	return true;
}

//-----------------------------------//

bool Win32Window::createWindow()
{
	DWORD style = WS_OVERLAPPEDWINDOW;
	DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	// TODO: fullscreen

	// AdjustWindowRectEx needs the rect to be filled in with the coordinates of 
	// the top-left and bottom-right corners of the desired client area
	RECT windowRect;
	SetRect( &windowRect, 0, 0, settings.getWidth(), settings.getHeight() );

	// AdjustWindowRectEx corrects the size of the client area of the window
	if (!AdjustWindowRectEx( &windowRect, style, false, exStyle ))
	{
		Log::error( "AdjustWindowRectEx() failed: %s", getErrorMessage().c_str() );
	}

	HWND hWnd = CreateWindowEx( exStyle, TEXT("vapor_Window"), TEXT("vaporEngine"), 
		style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, nullptr );

	if (!hWnd) 
	{
		Log::error( "CreateWindowEx() failed: %s", getErrorMessage().c_str() );
		return false;
	}

	createContext();
	makeCurrent();

	ShowWindow( hWnd, SW_SHOW );
	//SetForegroundWindow( hWnd );	// Slightly Higher Priority
	//SetFocus( hWnd );
	UpdateWindow( hWnd );

	return true;
}

//-----------------------------------//

bool Win32Window::createContext()
{
	hDC = GetDC( hWnd );

	if( !hDC )
	{
		Log::error( "GetDC() failed: %s", getErrorMessage().c_str() );
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd;
	memset( &pfd, 0, sizeof(pfd) );
	
	pfd.nSize        = sizeof( pfd );
	pfd.nVersion     = 1;
	pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType   = PFD_TYPE_RGBA;
	pfd.cColorBits   = 32;
	pfd.cStencilBits = 1;
	pfd.cDepthBits   = 32;

	const int pf = ChoosePixelFormat( hDC, &pfd );

	if ( !pf ) 
	{
		Log::error( "ChoosePixelFormat() failed: %s", getErrorMessage().c_str() );
	}

	if ( !SetPixelFormat( hDC, pf, &pfd ) ) 
	{
		Log::error( "SetPixelFormat() failed: %s", getErrorMessage().c_str() );
	} 

	//DescribePixelFormat( hDC, pf, sizeof ( PIXELFORMATDESCRIPTOR ), &pfd );

	//if ( pfd.dwFlags & PFD_NEED_PALETTE || pfd.dwFlags & PFD_NEED_SYSTEM_PALETTE )
	//{
	//	Log::error( "Requested format requires a palette");
	//}

	hRC = wglCreateContext( hDC );

	if( !hRC )
	{
		Log::error( "wglCreateContext() failed: %s", getErrorMessage().c_str() );
		return false;
	}

	return true;
}

//-----------------------------------//

string Win32Window::getErrorMessage()
{
	DWORD dw = GetLastError();

    LPTSTR lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, nullptr );

	puts( (LPCSTR) lpMsgBuf );

	string msg( (LPCSTR) lpMsgBuf );
	LocalFree(lpMsgBuf);

	return msg;
}

//-----------------------------------//

void Win32Window::update()
{
	if( hDC && hWnd && hRC )
	{
		SwapBuffers( hDC );
	}
}

//-----------------------------------//

bool Win32Window::pumpEvents()
{
	return true;
}

//-----------------------------------//

void Win32Window::setTitle(const string& title)
{

}


//-----------------------------------//

void Win32Window::setCursor(bool state)
{

}


//-----------------------------------//

void Win32Window::makeCurrent()
{
	if( wglGetCurrentContext() == hRC ) 
		return;

	if( !wglMakeCurrent(hDC, hRC) ) 
	{
		Log::error( "wglMakeCurrent() failed: %s", getErrorMessage().c_str() );
	}
}

//-----------------------------------//

LONG WINAPI WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{ 
  switch ( uMsg )
  {
	//case WM_SYSCOMMAND:	// Intercept System Commands
	//{
	//	switch ( wParam )	// Check System Calls
	//	{
	//	// Screensaver Trying To Start?
	//	case SC_SCREENSAVE:	
	//	
	//	// Monitor Trying To Enter Powersave?
	//	case SC_MONITORPOWER:
	//		return 0;	// Prevent From Happening
	//	}
	//	
	//	break;
	//}

    case WM_SIZE:
    {
		// this code would be an observer/subject pattern since
		// the user may want to do more things during resize
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);

		//settings.

		return 0;
    }

    case WM_CHAR:
    {
      // this wouldnt really be here, just for this example
	    switch ( wParam ) 
      {
        // escape key code
	      case 27:
	        PostQuitMessage( 0 );
	        break;
	    }
	    return 0;
    }

    case WM_CLOSE:
    {
		  PostQuitMessage( 0 );
	    return 0;
    }
  }

  return DefWindowProc( hWnd, uMsg, wParam, lParam ); 
} 

//-----------------------------------//

} // end namespace

#endif