/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/Win32_Window.h"

#ifdef VAPOR_WINDOWING_WIN32

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

static bool classRegistered = false;
static const TCHAR className[] = TEXT("vapor3D");

Win32Window::Win32Window(const WindowSettings& settings)
	: Window(settings)
	, hInstance(nullptr)
	, hDC(nullptr)
	, hWnd(nullptr)
	, hRC(nullptr)
{
	hInstance = GetModuleHandle(nullptr);

	if ( !registerClass() || !createWindow() ) 
	{
		LogError("Could not open a Win32 window");
		return;
	}
}

//-----------------------------------//

Win32Window::~Win32Window()
{
	removeViews();

	// Destroy context
	Deallocate(context);

	// Unregister class
	if( !UnregisterClass(className, NULL) )
	{
		LogError( "UnregisterClass() failed: %s", getErrorMessage().c_str() );
		return;
	}

	classRegistered = false;
}

//-----------------------------------//

bool Win32Window::registerClass()
{
	if(classRegistered) return true;

	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC ;
	wc.lpfnWndProc   = (WNDPROC) WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon( nullptr, IDI_APPLICATION );
	wc.hCursor       = LoadCursor( nullptr, IDC_ARROW );
	wc.hbrBackground = nullptr;
	wc.lpszMenuName  = nullptr;
	wc.lpszClassName = className;

	if( !RegisterClass(&wc) )
	{
		LogError( "RegisterClass() failed: %s", getErrorMessage().c_str() );
		return false;
	}

	classRegistered = true;
	return true;
}

//-----------------------------------//

bool Win32Window::createWindow()
{
	DWORD style = WS_OVERLAPPEDWINDOW;
	DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	// TODO: fullscreen

	// AdjustWindowRectEx needs the rect to be filled in with the coordinates
	// of the top-left and bottom-right corners of the desired client area.
	RECT windowRect;
	SetRect( &windowRect, 0, 0, settings.width, settings.height );

	// AdjustWindowRectEx corrects the size of the client area of the window
	if (!AdjustWindowRectEx( &windowRect, style, false, exStyle ))
	{
		LogError( "AdjustWindowRectEx() failed: %s", getErrorMessage().c_str() );
		return false;
	}

	hWnd = CreateWindowEx( exStyle, className, TEXT(""), 
		style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, hInstance, this );

	if (!hWnd) 
	{
		LogError( "CreateWindowEx() failed: %s", getErrorMessage().c_str() );
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
		LogError( "GetDC() failed: %s", getErrorMessage().c_str() );
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
		LogError( "ChoosePixelFormat() failed: %s", getErrorMessage().c_str() );
		return false;
	}

	if ( !SetPixelFormat( hDC, pf, &pfd ) ) 
	{
		LogError( "SetPixelFormat() failed: %s", getErrorMessage().c_str() );
		return false;
	} 

	//DescribePixelFormat( hDC, pf, sizeof ( PIXELFORMATDESCRIPTOR ), &pfd );

	//if ( pfd.dwFlags & PFD_NEED_PALETTE || pfd.dwFlags & PFD_NEED_SYSTEM_PALETTE )
	//{
	//	LogError( "Requested format requires a palette");
	//}

	hRC = wglCreateContext( hDC );

	if( !hRC )
	{
		LogError( "wglCreateContext() failed: %s", getErrorMessage().c_str() );
		return false;
	}

	context = AllocateThis(RenderContext);
	setContext(context);

	return true;
}

//-----------------------------------//

String Win32Window::getErrorMessage()
{
	DWORD dw = GetLastError();

	LPTSTR lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	String msg( (LPCSTR) lpMsgBuf );
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
	MSG msg;

	while( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
	{
		// If the message is WM_QUIT, then return.
		if (msg.message == WM_QUIT)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}

	return true;
}

//-----------------------------------//

bool Win32Window::setTitle(const std::wstring& title)
{
	// If the function succeeds, the return value is nonzero.
	if( !SetWindowText(hWnd, title.c_str()) )
	{
		LogError( "SetWindowText() failed: %s", getErrorMessage().c_str() );
		return false;
	}

	return true;
}

//-----------------------------------//

void Win32Window::setCursor(bool state)
{

}

//-----------------------------------//

WindowHandle* Win32Window::getHandle()
{
	return (WindowHandle*) hWnd;
}

//-----------------------------------//

void Win32Window::makeCurrent()
{
	if( wglGetCurrentContext() == hRC ) 
		return;

	if( !wglMakeCurrent(hDC, hRC) ) 
	{
		LogError( "wglMakeCurrent() failed: %s", getErrorMessage().c_str() );
		return;
	}
}

//-----------------------------------//

LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{ 
	Win32Window* window = (Win32Window*) GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch ( uMsg )
	{
	case WM_CREATE:
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*) lParam;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) pCreate->lpCreateParams);
		break;
	}
	case WM_SIZE:
	{
		Settings& settings = (Settings&) window->getSettings();
		settings.width = LOWORD(lParam);
		settings.height = HIWORD(lParam);
		window->handleWindowResize();
		break;
	}
	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam ); 
} 

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif