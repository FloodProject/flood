//============================================================================================
// Name: EntryForm.cpp
// Desc: Display settings dialog box
//============================================================================================

//--------------------------------------------------------------------------------------------
#include "EntryForm.h"

#ifdef COMPILE_WITH_ENTRY_FORM
#include <windows.h>
#include <ret_raster\IRaster.h>
//--------------------------------------------------------------------------------------------

using namespace ret_raster;
using namespace ret_device_setting;


//····························································································
unsigned int g_dwSDS = 0;
unsigned int g_dw_a_modes[] = { F_DEVICESETTING_D3D8, F_DEVICESETTING_D3D9 };
unsigned int g_dw_f_modes[] = { F_DEVICESETTING_FILTER_NONE, F_DEVICESETTING_FILTER_BILINEAL,
								F_DEVICESETTING_FILTER_TRILINEAL, F_DEVICESETTING_FILTER_ANISOTROPIC };
unsigned int g_dw_r_modes[] = { F_DEVICESETTING_640X480, F_DEVICESETTING_800X600,
								F_DEVICESETTING_1024X768, F_DEVICESETTING_1280X1024,
								F_DEVICESETTING_1600X1200 };
//····························································································


//--------------------------------------------------------------------------------------------
// Name: DialogProc()
// Desc: 
//--------------------------------------------------------------------------------------------
int __stdcall DialogProc( HWND hWnd, UN32 dwMsg, WPARAM wParam, LPARAM lParam )
{
    switch( dwMsg )
    {
		case WM_INITDIALOG:
		{
			// Fills driver combobox
			SendDlgItemMessage( hWnd, IDC_COMBO2, CB_ADDSTRING, 0, (DWORD)"Direct3D 8.1" );
			SendDlgItemMessage( hWnd, IDC_COMBO2, CB_ADDSTRING, 0, (DWORD)"Direct3D 9.0" );

			//  By default selection in Driver combobox
			if( g_dwSDS & F_DEVICESETTING_D3D8 )
				SendDlgItemMessage(hWnd, IDC_COMBO2, CB_SETCURSEL, 0, 0 );
			else if( g_dwSDS & F_DEVICESETTING_D3D9 )
					 SendDlgItemMessage(hWnd, IDC_COMBO2, CB_SETCURSEL, 1, 0 );
				 else
					 SendDlgItemMessage(hWnd, IDC_COMBO2, CB_SETCURSEL, 1, 0 );

			// Texture filter combobox
			SendDlgItemMessage( hWnd, IDC_COMBO3, CB_ADDSTRING, 0, (DWORD)"None" );
			SendDlgItemMessage( hWnd, IDC_COMBO3, CB_ADDSTRING, 0, (DWORD)"Bilineal" );
			SendDlgItemMessage( hWnd, IDC_COMBO3, CB_ADDSTRING, 0, (DWORD)"Trilineal" );
			SendDlgItemMessage( hWnd, IDC_COMBO3, CB_ADDSTRING, 0, (DWORD)"Anisotropic" );

			// By default selection int texture filter combobox
			if( g_dwSDS & F_DEVICESETTING_FILTER_NONE )
				SendDlgItemMessage(hWnd, IDC_COMBO3, CB_SETCURSEL, 0, 0 );
			else if( g_dwSDS & F_DEVICESETTING_FILTER_BILINEAL )
					 SendDlgItemMessage(hWnd, IDC_COMBO3, CB_SETCURSEL, 1, 0 );
				 else if( g_dwSDS & F_DEVICESETTING_FILTER_TRILINEAL )
						  SendDlgItemMessage(hWnd, IDC_COMBO3, CB_SETCURSEL, 2, 0 );
					  else if( g_dwSDS & F_DEVICESETTING_FILTER_ANISOTROPIC )
							   SendDlgItemMessage(hWnd, IDC_COMBO3, CB_SETCURSEL, 3, 0 );
						   else
							   SendDlgItemMessage(hWnd, IDC_COMBO3, CB_SETCURSEL, 2, 0 );

			// Screen size combobox
			SendDlgItemMessage( hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (DWORD)"6 4 0  x  4 8 0" );
			SendDlgItemMessage( hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (DWORD)"8 0 0  x  6 0 0" );
			SendDlgItemMessage( hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (DWORD)"1 0 2 4  x  7 6 8" );
			SendDlgItemMessage( hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (DWORD)"1 2 8 0  x  1 0 2 4" );
			SendDlgItemMessage( hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (DWORD)"1 6 0 0  x  1 2 0 0" );

			// By default selection in screen size combobox
			if( g_dwSDS & F_DEVICESETTING_640X480 )
				SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETCURSEL, 0, 0 );
				else if( g_dwSDS & F_DEVICESETTING_800X600 )
						 SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETCURSEL, 1, 0 );
					else if( g_dwSDS & F_DEVICESETTING_1024X768 )
							 SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETCURSEL, 2, 0 );
						else if( g_dwSDS & F_DEVICESETTING_1280X1024 )
								 SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETCURSEL, 3, 0 );
								else if( g_dwSDS & F_DEVICESETTING_1600X1200 )
										 SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETCURSEL, 4, 0 );
									 else
										 SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETCURSEL, 1, 0 );

			if( g_dwSDS & F_DEVICESETTING_WINDOWED )
				SendDlgItemMessage( hWnd, IDC_CHECK6, BM_SETCHECK, BST_CHECKED, 0 );

			if( g_dwSDS & F_DEVICESETTING_ANTIALIAS )
				SendDlgItemMessage( hWnd, IDC_CHECK1, BM_SETCHECK, BST_CHECKED, 0 );

			if( g_dwSDS & F_DEVICESETTING_BACKBUFFER_16 )
				SendDlgItemMessage( hWnd, IDC_CHECK3, BM_SETCHECK, BST_CHECKED, 0 );

			if( g_dwSDS & F_DEVICESETTING_DEPTHBUFFER_16 )
				SendDlgItemMessage( hWnd, IDC_CHECK4, BM_SETCHECK, BST_CHECKED, 0 );

			if( g_dwSDS & F_DEVICESETTING_VSYNC )
				SendDlgItemMessage( hWnd, IDC_CHECK2, BM_SETCHECK, BST_CHECKED, 0 );

			break;
		}
	
		case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
		    {
				case IDOK: // Fill the last value for "g_dwSDS"
				{
					g_dwSDS = 0x00000000;

					g_dwSDS |= g_dw_r_modes[SendDlgItemMessage(hWnd, IDC_COMBO1, CB_GETCURSEL, 0, 0 )];
					g_dwSDS |= g_dw_a_modes[SendDlgItemMessage(hWnd, IDC_COMBO2, CB_GETCURSEL, 0, 0 )];
					g_dwSDS |= g_dw_f_modes[SendDlgItemMessage(hWnd, IDC_COMBO3, CB_GETCURSEL, 0, 0 )];
					
					if( SendDlgItemMessage( hWnd, IDC_CHECK6, BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						g_dwSDS |= F_DEVICESETTING_WINDOWED;

					if( SendDlgItemMessage( hWnd, IDC_CHECK1, BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						g_dwSDS |= F_DEVICESETTING_ANTIALIAS;

					if( SendDlgItemMessage( hWnd, IDC_CHECK3, BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						g_dwSDS |= F_DEVICESETTING_BACKBUFFER_16;

					if( SendDlgItemMessage( hWnd, IDC_CHECK4, BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						g_dwSDS |= F_DEVICESETTING_DEPTHBUFFER_16;

					if( SendDlgItemMessage( hWnd, IDC_CHECK2, BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						g_dwSDS |= F_DEVICESETTING_VSYNC;

					EndDialog( hWnd, TRUE );
					break;
				}

		        case IDCANCEL:
				{
					EndDialog( hWnd, FALSE );
					break;
				}
			}

			break;
		}
    }

    return 0; 
}


//--------------------------------------------------------------------------------------------
// Name: EntryForm()
// Desc: 
//--------------------------------------------------------------------------------------------
bool EntryForm( unsigned int &dwSDS )
{
	int iRes;

	g_dwSDS = dwSDS;
	iRes = DialogBox( GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), GetDesktopWindow(),
					 (DLGPROC)DialogProc );
	dwSDS = g_dwSDS;

	return iRes > 0;
}


#endif // COMPILE_WITH_ENTRY_FORM
