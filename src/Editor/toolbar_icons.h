/*
	Automatic generated header by:

		wxInclude by Kim De Deyn, use --help for more information.
		Version 1.0, compiled at Sep 12 2007 17:26:17

	Header: toolbar_icons
	Macros: yes
	Const: no
*/

#ifndef _WXINCLUDE_TOOLBAR_ICONS_H_
#define _WXINCLUDE_TOOLBAR_ICONS_H_

#include "wx/wx.h"
#include "wx/mstream.h"

#define wxMEMORY_IMAGE( name ) _wxConvertMemoryToImage( name, sizeof( name ) )
#define wxMEMORY_IMAGEEX( name, type ) _wxConvertMemoryToImage( name, sizeof( name ), type )
#define wxMEMORY_BITMAP( name ) _wxConvertMemoryToBitmap( name, sizeof( name ) )
#define wxMEMORY_BITMAPEX( name, type ) _wxConvertMemoryToBitmap( name, sizeof( name ), type )

inline wxImage _wxConvertMemoryToImage(const unsigned char* data, int length, long type = wxBITMAP_TYPE_ANY )
{
	wxMemoryInputStream stream( data, length );
	return wxImage( stream, type, -1 );
}

inline wxBitmap _wxConvertMemoryToBitmap(const unsigned char* data, int length, long type = wxBITMAP_TYPE_ANY )
{
	wxMemoryInputStream stream( data, length );
	return wxBitmap( wxImage( stream, type, -1 ), -1 );
}

static unsigned char application_side_tree[] = {
0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 
0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 
0x00, 0x00, 0x00, 0x10, 0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 
0xF3, 0xFF, 0x61, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 
0x41, 0x00, 0x00, 0xAF, 0xC8, 0x37, 0x05, 0x8A, 0xE9, 0x00, 
0x00, 0x00, 0x19, 0x74, 0x45, 0x58, 0x74, 0x53, 0x6F, 0x66, 
0x74, 0x77, 0x61, 0x72, 0x65, 0x00, 0x41, 0x64, 0x6F, 0x62, 
0x65, 0x20, 0x49, 0x6D, 0x61, 0x67, 0x65, 0x52, 0x65, 0x61, 
0x64, 0x79, 0x71, 0xC9, 0x65, 0x3C, 0x00, 0x00, 0x01, 0x75, 
0x49, 0x44, 0x41, 0x54, 0x18, 0x19, 0xA5, 0xC1, 0xB1, 0x6A, 
0x54, 0x41, 0x14, 0xC7, 0xE1, 0xDF, 0x9C, 0x99, 0x1B, 0x02, 
0xBB, 0xDA, 0x6A, 0xEB, 0x3B, 0x6C, 0xB1, 0xA0, 0x85, 0xC5, 
0xBC, 0x80, 0x4D, 0x1A, 0x4B, 0x2B, 0xB1, 0xB2, 0x12, 0x14, 
0x6B, 0xB1, 0x31, 0x10, 0xC8, 0x1B, 0x08, 0x62, 0x0A, 0xDB, 
0x6D, 0xAE, 0xAF, 0x61, 0x0A, 0x05, 0xB1, 0x48, 0xBB, 0x55, 
0xB2, 0x8A, 0xF7, 0x9E, 0xF3, 0x77, 0x47, 0x0C, 0x5C, 0x82, 
0x04, 0xC2, 0x7E, 0x5F, 0x92, 0xC4, 0x2E, 0xD2, 0xA3, 0xE7, 
0x1F, 0x9F, 0x3D, 0x78, 0xB8, 0x78, 0xBB, 0xDE, 0x68, 0x26, 
0x01, 0x11, 0x28, 0x02, 0x0F, 0x47, 0x21, 0x42, 0x81, 0xDC, 
0x09, 0x09, 0xB9, 0xE3, 0x11, 0xDC, 0xDE, 0x1B, 0x2E, 0x4E, 
0x4F, 0xCF, 0x5E, 0x7C, 0x7A, 0x77, 0x70, 0x5C, 0x16, 0xF7, 
0x17, 0x47, 0xDD, 0xFE, 0xCC, 0xEE, 0xEC, 0x73, 0x13, 0xB3, 
0x8B, 0x8D, 0x1F, 0x01, 0xC7, 0x65, 0x33, 0xC8, 0x36, 0xEB, 
0x73, 0x6E, 0xEA, 0xE7, 0x20, 0xA3, 0x79, 0xF9, 0xFE, 0xAB, 
0xFE, 0xE7, 0xD5, 0x87, 0x6F, 0xBA, 0xCE, 0x93, 0x37, 0x9F, 
0xB5, 0x45, 0x91, 0x82, 0x66, 0xB5, 0x5A, 0xD1, 0x75, 0x1D, 
0xB5, 0x56, 0xA6, 0xD6, 0xE7, 0xBF, 0xF0, 0x51, 0xF8, 0x28, 
0x5C, 0xC1, 0x6F, 0x17, 0xF7, 0xEE, 0xDE, 0xC2, 0xC3, 0x69, 
0x0A, 0x12, 0x4D, 0xD7, 0x75, 0xD4, 0x5A, 0xB9, 0xAA, 0xE4, 
0x42, 0x4A, 0x4E, 0xCA, 0x22, 0x87, 0x51, 0x46, 0xD1, 0xC4, 
0xE8, 0x34, 0xC5, 0xC3, 0xB9, 0xD4, 0xF7, 0x3D, 0x4D, 0xAD, 
0x95, 0x4B, 0xD9, 0xC0, 0x52, 0xA6, 0x24, 0xF0, 0x24, 0xDC, 
0x82, 0x26, 0x3C, 0x68, 0x4C, 0x21, 0x9A, 0xE5, 0x72, 0x49, 
0x53, 0x6B, 0x65, 0x2A, 0x27, 0x30, 0x03, 0xCB, 0x90, 0x4B, 
0x62, 0x2F, 0x67, 0x9A, 0x08, 0xA7, 0xB1, 0x08, 0x67, 0xAA, 
0xEF, 0x7B, 0xA6, 0x52, 0x31, 0x8A, 0x41, 0x36, 0x28, 0x19, 
0x2C, 0xF3, 0x57, 0x44, 0xD0, 0x94, 0x71, 0x18, 0x98, 0xAA, 
0xB5, 0x32, 0x65, 0x6C, 0x99, 0x91, 0x01, 0x97, 0xB0, 0x24, 
0x9A, 0x71, 0x18, 0x69, 0x4C, 0x1E, 0x4C, 0xF5, 0x7D, 0xCF, 
0x94, 0xA5, 0x84, 0xA5, 0x04, 0x29, 0x91, 0xCD, 0xC8, 0x29, 
0xD1, 0x28, 0x82, 0xA6, 0x44, 0x04, 0xCD, 0x7C, 0x3E, 0xA7, 
0xD6, 0xCA, 0x55, 0x39, 0x25, 0x9A, 0xCC, 0x3F, 0x39, 0xD3, 
0x84, 0x44, 0x53, 0xCE, 0xBE, 0xFF, 0x38, 0x79, 0x7A, 0x38, 
0x1C, 0x84, 0x3B, 0xE1, 0x81, 0xBB, 0x13, 0x11, 0xF8, 0x30, 
0xF0, 0xF8, 0xF5, 0x17, 0x42, 0x42, 0xE1, 0xC8, 0x45, 0x28, 
0x50, 0x04, 0x11, 0x34, 0x27, 0x6C, 0x25, 0x49, 0xEC, 0xC2, 
0xD8, 0xD1, 0x1F, 0xA8, 0x93, 0xF9, 0xFF, 0xB3, 0xFD, 0x4B, 
0x0A, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 
0x42, 0x60, 0x82
};

static unsigned char disk[] = {
0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 
0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 
0x00, 0x00, 0x00, 0x10, 0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 
0xF3, 0xFF, 0x61, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 
0x41, 0x00, 0x00, 0xAF, 0xC8, 0x37, 0x05, 0x8A, 0xE9, 0x00, 
0x00, 0x00, 0x19, 0x74, 0x45, 0x58, 0x74, 0x53, 0x6F, 0x66, 
0x74, 0x77, 0x61, 0x72, 0x65, 0x00, 0x41, 0x64, 0x6F, 0x62, 
0x65, 0x20, 0x49, 0x6D, 0x61, 0x67, 0x65, 0x52, 0x65, 0x61, 
0x64, 0x79, 0x71, 0xC9, 0x65, 0x3C, 0x00, 0x00, 0x01, 0xFE, 
0x49, 0x44, 0x41, 0x54, 0x18, 0x19, 0x05, 0xC1, 0x3D, 0x8B, 
0x5D, 0x55, 0x18, 0x06, 0xD0, 0xB5, 0xCF, 0xEC, 0x3B, 0x43, 
0x82, 0x92, 0xA8, 0x60, 0x84, 0x44, 0x44, 0x03, 0x0A, 0x56, 
0x01, 0x65, 0x20, 0xF1, 0xA3, 0x15, 0x7F, 0x82, 0x85, 0x68, 
0x91, 0x2A, 0x42, 0xD0, 0x22, 0xA5, 0x36, 0x56, 0x56, 0x22, 
0xA2, 0x08, 0x63, 0xA1, 0xD8, 0x0A, 0x76, 0x82, 0x62, 0x61, 
0x95, 0x22, 0x16, 0x16, 0xC1, 0x4E, 0xC5, 0x42, 0xB4, 0x12, 
0xC5, 0x0C, 0xDE, 0x99, 0x7B, 0xF6, 0x7E, 0x1F, 0xD7, 0x6A, 
0x87, 0x6F, 0x7C, 0xF7, 0x16, 0x1E, 0xC3, 0xF3, 0x78, 0x02, 
0xF7, 0x91, 0x8E, 0x26, 0xC0, 0xDE, 0xD2, 0x2C, 0x4B, 0x93, 
0x94, 0x2A, 0xFF, 0xEC, 0x6F, 0xFC, 0xBC, 0x3D, 0x5D, 0xDF, 
0xBB, 0xF3, 0xF1, 0xCB, 0x5F, 0x76, 0x3C, 0x7D, 0x74, 0xEB, 
0xDA, 0x75, 0x9A, 0xA7, 0x2E, 0x1E, 0x08, 0x52, 0xB1, 0x1B, 
0xB1, 0x5B, 0xCB, 0xDE, 0xD2, 0x9C, 0x39, 0x58, 0xB4, 0xC6, 
0xF1, 0xB6, 0x1C, 0x9F, 0xCC, 0x73, 0xBF, 0xFE, 0x79, 0x7C, 
0xE5, 0xF3, 0x6F, 0x7E, 0xF9, 0xE4, 0xF0, 0xC6, 0xD7, 0xE7, 
0x16, 0xBC, 0x20, 0x7C, 0x75, 0xE7, 0x5F, 0x15, 0x92, 0xB8, 
0xF9, 0xE1, 0x0F, 0xAA, 0x42, 0x6B, 0x36, 0xBD, 0x59, 0x96, 
0x66, 0x37, 0x62, 0xBB, 0x96, 0x51, 0xDA, 0xB4, 0xF4, 0xD7, 
0x5E, 0xBA, 0xFC, 0xD0, 0x99, 0x83, 0xFD, 0x8F, 0x16, 0x3C, 
0x12, 0x54, 0xC5, 0x3A, 0xCA, 0x3A, 0x02, 0xD6, 0x8A, 0xBE, 
0xC7, 0xA6, 0x37, 0x55, 0xB1, 0xDD, 0x95, 0x75, 0x84, 0xC4, 
0xF7, 0x77, 0xEF, 0xD9, 0xDF, 0xEC, 0xD9, 0x9E, 0x8E, 0xFD, 
0x8E, 0xB3, 0xA3, 0x82, 0xD8, 0xEE, 0x0A, 0x2C, 0xAD, 0x11, 
0x36, 0x7D, 0xD1, 0x34, 0x27, 0x6B, 0xD9, 0x9E, 0x46, 0x25, 
0x2A, 0x54, 0xE2, 0x8F, 0xBF, 0x4E, 0x48, 0x5A, 0x27, 0x7D, 
0x26, 0x12, 0x4E, 0x47, 0xC0, 0xDB, 0xAF, 0x5F, 0xB1, 0xE9, 
0x8B, 0xBE, 0xB0, 0xCE, 0xF8, 0xEF, 0xB4, 0x8C, 0x2A, 0x55, 
0x44, 0xA4, 0xCA, 0x3A, 0xA6, 0xA4, 0x74, 0xB4, 0x9A, 0x48, 
0xBC, 0xFB, 0xD9, 0x8F, 0x5A, 0x6B, 0x96, 0xC6, 0xA6, 0x2F, 
0x1A, 0x46, 0xC5, 0x18, 0x25, 0x00, 0x38, 0x7F, 0xFE, 0x41, 
0xEB, 0xD8, 0x23, 0xD1, 0x85, 0x4A, 0xA9, 0x9A, 0x1E, 0xBE, 
0x70, 0x91, 0x46, 0x13, 0x02, 0x51, 0xA1, 0xAA, 0x24, 0x51, 
0x89, 0xAA, 0xA2, 0x35, 0xEB, 0x3A, 0x48, 0xE9, 0x30, 0x27, 
0x09, 0xD0, 0x12, 0xA0, 0x51, 0x45, 0x12, 0x44, 0x82, 0x44, 
0x4B, 0x24, 0x65, 0x5D, 0x9B, 0xA4, 0x74, 0x18, 0xB3, 0x10, 
0x10, 0xD1, 0x34, 0x12, 0x49, 0xA4, 0x4A, 0x44, 0x12, 0x55, 
0x53, 0x2A, 0xA2, 0x9C, 0xAE, 0x21, 0xA5, 0x13, 0xB3, 0x42, 
0x78, 0xE7, 0x95, 0x4B, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xD6, 
0xD1, 0x4F, 0xC6, 0x88, 0xA4, 0xF4, 0xA5, 0x35, 0xB3, 0x88, 
0x48, 0xF8, 0xE0, 0xF6, 0x75, 0x00, 0x00, 0x00, 0x70, 0xF3, 
0xEA, 0x91, 0x39, 0x87, 0xDD, 0x08, 0x89, 0xAE, 0x51, 0x15, 
0x0D, 0x70, 0xED, 0xD2, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x6A, 
0x96, 0xB9, 0x06, 0xA5, 0x27, 0xC9, 0xAC, 0x34, 0x02, 0x6E, 
0xFF, 0xFE, 0x05, 0x00, 0x00, 0x00, 0x78, 0xF6, 0xD1, 0x17, 
0xCD, 0x39, 0xAD, 0x33, 0x92, 0xA4, 0x57, 0xFC, 0xB6, 0x1B, 
0xF5, 0xF8, 0x33, 0x97, 0xCF, 0x6A, 0x8D, 0x37, 0x9F, 0xFB, 
0x14, 0x00, 0x00, 0x00, 0xC0, 0x85, 0xFB, 0xCB, 0x9C, 0x81, 
0xBF, 0xDB, 0xE1, 0x8D, 0x6F, 0xDF, 0x4F, 0xF2, 0x24, 0xB9, 
0x2A, 0x79, 0x80, 0x92, 0x84, 0x94, 0xA4, 0x48, 0x49, 0x8A, 
0x14, 0x89, 0x24, 0x48, 0xC8, 0x3D, 0x6D, 0xB9, 0xFB, 0x3F, 
0x21, 0x7C, 0x6E, 0x8B, 0x6D, 0x10, 0xB0, 0xA2, 0x00, 0x00, 
0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82

};

static unsigned char folder_explore[] = {
0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 
0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 
0x00, 0x00, 0x00, 0x10, 0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 
0xF3, 0xFF, 0x61, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 
0x41, 0x00, 0x00, 0xAF, 0xC8, 0x37, 0x05, 0x8A, 0xE9, 0x00, 
0x00, 0x00, 0x19, 0x74, 0x45, 0x58, 0x74, 0x53, 0x6F, 0x66, 
0x74, 0x77, 0x61, 0x72, 0x65, 0x00, 0x41, 0x64, 0x6F, 0x62, 
0x65, 0x20, 0x49, 0x6D, 0x61, 0x67, 0x65, 0x52, 0x65, 0x61, 
0x64, 0x79, 0x71, 0xC9, 0x65, 0x3C, 0x00, 0x00, 0x02, 0x39, 
0x49, 0x44, 0x41, 0x54, 0x18, 0x19, 0xA5, 0xC1, 0x3D, 0x48, 
0x55, 0x61, 0x00, 0xC7, 0xE1, 0xDF, 0x7B, 0xEE, 0x7B, 0x8F, 
0x57, 0xF3, 0x98, 0xA1, 0xD6, 0xD5, 0x08, 0xD1, 0xC5, 0xDB, 
0x17, 0xB4, 0x24, 0x08, 0x0E, 0x41, 0x41, 0x50, 0x2D, 0x6D, 
0x4D, 0xD5, 0x50, 0x2D, 0xCE, 0xD1, 0xD6, 0xC7, 0x58, 0x73, 
0x50, 0x53, 0x11, 0x34, 0x35, 0x06, 0x11, 0x11, 0x45, 0x34, 
0x06, 0x81, 0xD4, 0x60, 0x1F, 0x6A, 0x54, 0x94, 0x5D, 0x2D, 
0xEF, 0xD5, 0xFB, 0xA1, 0xE7, 0x9C, 0xF7, 0xBC, 0xFF, 0x8C, 
0x6E, 0xE4, 0xD2, 0x10, 0x3E, 0x8F, 0x91, 0xC4, 0x46, 0xD8, 
0x77, 0x0F, 0xF6, 0x4F, 0x16, 0xBA, 0x76, 0xED, 0x32, 0x26, 
0xE0, 0x2F, 0x11, 0xD7, 0x66, 0x3E, 0x67, 0xAE, 0x3A, 0x06, 
0xC4, 0xAC, 0x67, 0xF8, 0xA5, 0x31, 0x72, 0xEC, 0xB5, 0x63, 
0x8D, 0x35, 0x0A, 0x4A, 0x3B, 0xC6, 0x6F, 0xE7, 0x8D, 0x31, 
0x20, 0xD6, 0x08, 0x8C, 0xC1, 0xAD, 0x7E, 0x1F, 0x2E, 0x4F, 
0x5E, 0xF9, 0x2C, 0x10, 0x2D, 0xB9, 0xB0, 0x4B, 0xCA, 0xD2, 
0xF9, 0xFA, 0xDC, 0xD3, 0xF3, 0xC0, 0x3D, 0xD6, 0x58, 0x9C, 
0x56, 0xF1, 0x49, 0x5B, 0xFC, 0xED, 0x06, 0x59, 0x92, 0x83, 
0xA0, 0x0B, 0x63, 0x22, 0x0A, 0x03, 0x07, 0x19, 0x18, 0xBD, 
0x1A, 0x22, 0x01, 0x1E, 0xE4, 0x01, 0x61, 0x82, 0xCE, 0xE2, 
0xFB, 0xFB, 0xE3, 0x17, 0x81, 0x7B, 0xAC, 0xB1, 0xA4, 0x26, 
0x90, 0x4F, 0xC8, 0x92, 0x10, 0x79, 0xE1, 0x57, 0x3E, 0x91, 
0x36, 0xCA, 0xD4, 0xE7, 0x9F, 0x91, 0x2F, 0xF4, 0x83, 0x52, 
0xF0, 0x09, 0x92, 0x03, 0x1C, 0x51, 0xE9, 0x42, 0x88, 0xFC, 
0x20, 0x2D, 0x16, 0xA3, 0x82, 0x77, 0x2B, 0x24, 0x4D, 0x50, 
0xD2, 0x20, 0xAD, 0x2D, 0x13, 0x95, 0x4E, 0x51, 0xE8, 0xD9, 
0x09, 0x18, 0x40, 0x80, 0x07, 0x09, 0x10, 0xC6, 0x76, 0x43, 
0x16, 0x58, 0x5A, 0x2C, 0x69, 0x80, 0x6D, 0xDF, 0x4A, 0xF7, 
0xCE, 0x33, 0x80, 0x07, 0x09, 0xE1, 0xF1, 0xC9, 0x17, 0x5C, 
0xF5, 0x09, 0xD3, 0x8B, 0xBD, 0x7C, 0x58, 0x1E, 0x64, 0x61, 
0xA5, 0x48, 0x7B, 0x5B, 0x9E, 0x9E, 0x4D, 0x75, 0x06, 0xDC, 
0x16, 0x43, 0x8B, 0x25, 0x95, 0x94, 0xAD, 0xE2, 0x9B, 0xAF, 
0x50, 0xB6, 0x84, 0xB2, 0x1A, 0x72, 0x55, 0x7C, 0x5A, 0x61, 
0xBA, 0x52, 0xE4, 0x63, 0x7A, 0x80, 0xD2, 0x50, 0x27, 0x7B, 
0xF2, 0x39, 0xE6, 0x97, 0x12, 0x2A, 0xB5, 0x98, 0xD9, 0xCE, 
0xD3, 0x76, 0x37, 0xBF, 0x05, 0xC4, 0x48, 0xDE, 0xE1, 0xD3, 
0x05, 0x7C, 0xB2, 0x80, 0x4F, 0xCA, 0xF8, 0xA4, 0x8C, 0xD2, 
0x05, 0xA6, 0xEA, 0x07, 0x19, 0xDE, 0xD6, 0x41, 0x25, 0x36, 
0xCC, 0x2D, 0x67, 0x38, 0x93, 0x63, 0xFB, 0xD6, 0x88, 0xE6, 
0xE6, 0xB1, 0x1C, 0x2D, 0x96, 0x44, 0x32, 0x64, 0xC8, 0x2D, 
0x22, 0x57, 0x45, 0xAE, 0x8A, 0xB2, 0x25, 0xE4, 0x96, 0xA8, 
0xAF, 0x24, 0x84, 0x61, 0x1E, 0x17, 0x3B, 0xBC, 0x20, 0x76, 
0xA2, 0x60, 0x73, 0x38, 0x59, 0xFE, 0xB0, 0x4A, 0x7C, 0xC5, 
0x35, 0x7F, 0x74, 0xA4, 0xF5, 0x30, 0xC2, 0x47, 0x81, 0x94, 
0x07, 0x1F, 0x81, 0x7A, 0xE9, 0x68, 0xCB, 0x93, 0xA4, 0x19, 
0x69, 0x06, 0xCE, 0x8B, 0x5F, 0x64, 0xA0, 0xD9, 0x68, 0xF0, 
0x87, 0xD5, 0x6A, 0x7C, 0x6D, 0xF6, 0xE6, 0xF1, 0x51, 0xC4, 
0x21, 0x0C, 0x11, 0xEB, 0x44, 0x23, 0xC3, 0xF9, 0x5A, 0xF1, 
0x48, 0x2E, 0x6A, 0x0F, 0x89, 0x33, 0x61, 0x03, 0xC3, 0xDC, 
0x62, 0x8A, 0x7E, 0xBC, 0xD5, 0xA3, 0x4B, 0x13, 0xA7, 0x0E, 
0x5F, 0x79, 0x7C, 0xC7, 0x48, 0xE2, 0x5F, 0xAE, 0x3F, 0xFC, 
0x38, 0xD4, 0x66, 0x39, 0xB9, 0xA5, 0x33, 0xBC, 0xEC, 0x05, 
0xCB, 0x8D, 0xD8, 0xBB, 0xF2, 0x94, 0x19, 0xEF, 0xFB, 0x6A, 
0x1A, 0x33, 0xCF, 0x6B, 0xB3, 0x53, 0x2F, 0xCF, 0x19, 0x49, 
0xFC, 0x8F, 0xFB, 0x97, 0x8F, 0x9E, 0x2D, 0xF6, 0x17, 0xAF, 
0xF5, 0x0D, 0xED, 0xEB, 0x7E, 0xF3, 0xE2, 0x61, 0xD3, 0x48, 
0xE2, 0x7F, 0xDD, 0x9D, 0x28, 0x9D, 0xE8, 0x1F, 0xDE, 0x7B, 
0xCB, 0x7B, 0xD7, 0x6E, 0x24, 0xB1, 0x11, 0x3F, 0x01, 0x54, 
0xA7, 0x36, 0x4C, 0xD8, 0x15, 0x9A, 0xB6, 0x00, 0x00, 0x00, 
0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

static unsigned char page[] = {
0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 
0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 
0x00, 0x00, 0x00, 0x10, 0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 
0xF3, 0xFF, 0x61, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 
0x41, 0x00, 0x00, 0xAF, 0xC8, 0x37, 0x05, 0x8A, 0xE9, 0x00, 
0x00, 0x00, 0x19, 0x74, 0x45, 0x58, 0x74, 0x53, 0x6F, 0x66, 
0x74, 0x77, 0x61, 0x72, 0x65, 0x00, 0x41, 0x64, 0x6F, 0x62, 
0x65, 0x20, 0x49, 0x6D, 0x61, 0x67, 0x65, 0x52, 0x65, 0x61, 
0x64, 0x79, 0x71, 0xC9, 0x65, 0x3C, 0x00, 0x00, 0x02, 0x0D, 
0x49, 0x44, 0x41, 0x54, 0x18, 0x19, 0x05, 0xC1, 0xBF, 0xAF, 
0x9E, 0x73, 0x18, 0x07, 0xE0, 0xEB, 0xFE, 0x3E, 0xCF, 0x7B, 
0xDA, 0xD3, 0x53, 0x47, 0x95, 0x88, 0x33, 0x11, 0xF1, 0x3B, 
0x31, 0xD0, 0x88, 0x18, 0x84, 0x85, 0x81, 0x2E, 0x06, 0x93, 
0xC5, 0x62, 0x63, 0x23, 0x36, 0x31, 0xD9, 0x6C, 0x92, 0x8E, 
0x06, 0x49, 0xFF, 0x03, 0xA3, 0x41, 0x4B, 0x44, 0x22, 0x24, 
0x82, 0x18, 0x58, 0x24, 0x0D, 0xDA, 0x54, 0x1B, 0x44, 0xCF, 
0x79, 0xCF, 0xF3, 0xDC, 0x1F, 0xD7, 0x55, 0x49, 0xBC, 0xF0, 
0xDE, 0xA5, 0xD7, 0x77, 0xF7, 0x4F, 0xBC, 0x36, 0xCA, 0x39, 
0x9C, 0x44, 0x81, 0x42, 0x8A, 0xB4, 0xA5, 0xFB, 0xD7, 0x9B, 
0xD7, 0xF3, 0x59, 0xFD, 0x7D, 0xE3, 0x9D, 0xCB, 0x17, 0xCE, 
0x6F, 0x01, 0x2A, 0x89, 0x97, 0x3E, 0xF8, 0xF2, 0xF2, 0x27, 
0x6F, 0x9D, 0x7B, 0x74, 0x8C, 0x3A, 0x13, 0x26, 0x01, 0x20, 
0x81, 0xB8, 0xF6, 0xEF, 0xB1, 0x8B, 0xDF, 0xDC, 0xF4, 0xC7, 
0x6F, 0x47, 0xFF, 0xFD, 0xF4, 0xC3, 0xCF, 0x67, 0xBF, 0xB8, 
0x70, 0xFE, 0x08, 0x66, 0x18, 0x53, 0x3D, 0xBC, 0x33, 0x4F, 
0x67, 0x7E, 0xBC, 0x6A, 0x1A, 0x55, 0x14, 0x69, 0xD6, 0xB4, 
0x65, 0x89, 0x5B, 0xC7, 0xAB, 0xE7, 0x1F, 0xBC, 0xCD, 0xD3, 
0x0F, 0xCD, 0xBE, 0xEE, 0x6B, 0xA7, 0xD2, 0x8F, 0xDC, 0x78, 
0xEE, 0xCD, 0x4F, 0xEF, 0xF8, 0xFC, 0xA3, 0x97, 0x8F, 0x06, 
0x54, 0xD5, 0xE9, 0x24, 0xD3, 0xCE, 0x34, 0x6C, 0x06, 0x73, 
0x31, 0x0D, 0xA6, 0xA2, 0x8A, 0xA1, 0xD0, 0x0E, 0xF6, 0x77, 
0x9C, 0x7F, 0xEA, 0xC0, 0x7D, 0x0F, 0x9C, 0xD8, 0x9D, 0xF7, 
0xF7, 0x3E, 0x86, 0x01, 0xA1, 0x60, 0x60, 0x14, 0xA3, 0xCA, 
0xA8, 0x32, 0xAA, 0x8C, 0x2A, 0x63, 0x2A, 0xBF, 0xFF, 0x75, 
0xCB, 0xBD, 0xA7, 0x8F, 0x3D, 0x76, 0xB6, 0xBC, 0xF1, 0xCC, 
0x9D, 0xA6, 0xCD, 0xF4, 0x2A, 0xCC, 0x00, 0x09, 0xF3, 0x44, 
0x29, 0xA9, 0xD2, 0x6B, 0x18, 0x65, 0x9E, 0xD8, 0xC9, 0xF0, 
0xCB, 0xF5, 0xD8, 0xF6, 0xA2, 0x7B, 0xEB, 0xD9, 0xFB, 0x4F, 
0xA8, 0x79, 0x2C, 0x30, 0x43, 0x42, 0x30, 0xD7, 0x10, 0x11, 
0xD4, 0x28, 0x85, 0x34, 0x99, 0x06, 0x3B, 0x8C, 0x2E, 0xDD, 
0x93, 0x18, 0xD4, 0x00, 0x33, 0x04, 0x8D, 0x2A, 0x4A, 0x09, 
0x82, 0x34, 0x9B, 0x09, 0x50, 0x43, 0xAD, 0x25, 0x23, 0x1A, 
0x09, 0x30, 0x43, 0x2F, 0x24, 0x34, 0x0A, 0x49, 0xC0, 0x18, 
0xE8, 0x32, 0x4F, 0x21, 0x45, 0x88, 0xB2, 0x36, 0x6B, 0x03, 
0x33, 0x74, 0x47, 0x12, 0x42, 0x04, 0x80, 0x00, 0xA5, 0x54, 
0x98, 0x07, 0x8D, 0x93, 0x1B, 0xD6, 0x25, 0x60, 0x86, 0xB5, 
0x59, 0x43, 0x9A, 0x06, 0x10, 0xD0, 0x61, 0x6D, 0x82, 0x0E, 
0x29, 0x36, 0xD3, 0xB0, 0x6C, 0x17, 0x30, 0x43, 0x2F, 0xD1, 
0x89, 0xF7, 0xBF, 0xDB, 0x2A, 0xD1, 0x45, 0x52, 0x34, 0xA9, 
0x80, 0x84, 0x06, 0x5C, 0xBC, 0x67, 0xE3, 0xF8, 0xA8, 0xC1, 
0x0C, 0xCB, 0xB2, 0x5A, 0x57, 0x0E, 0xF6, 0x37, 0x9E, 0xBC, 
0x7B, 0xE8, 0xB0, 0x86, 0xB5, 0xE8, 0x10, 0x24, 0x24, 0x7C, 
0x7F, 0x75, 0x91, 0x8E, 0xE3, 0xC3, 0x05, 0xCC, 0xB0, 0x1C, 
0x75, 0x2F, 0xDD, 0xD9, 0xAE, 0xEA, 0xAB, 0x2B, 0xAB, 0xAE, 
0x88, 0x02, 0x1D, 0x4A, 0x74, 0xC8, 0x28, 0xA3, 0xCB, 0x3C, 
0x57, 0x2F, 0x6B, 0x2F, 0x30, 0xC3, 0xF1, 0xE1, 0xFA, 0xE7, 
0xDE, 0xCE, 0x74, 0xD7, 0xBB, 0x8F, 0x6F, 0xF7, 0x92, 0x0C, 
0xA1, 0x01, 0xB7, 0xEF, 0xCE, 0x20, 0x61, 0x77, 0x53, 0xAA, 
0xF4, 0x66, 0xAA, 0x7F, 0xD6, 0xA5, 0xAF, 0xC0, 0x0C, 0x55, 
0xF9, 0xF0, 0xC5, 0xB7, 0x2F, 0xBD, 0x52, 0x55, 0x4F, 0x44, 
0x4E, 0x01, 0x08, 0x41, 0x04, 0x08, 0x38, 0xEC, 0xEE, 0x6F, 
0xE7, 0xE1, 0x22, 0xFC, 0x0F, 0x44, 0x38, 0x1E, 0x56, 0x35, 
0x83, 0x6A, 0x12, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 
0x44, 0xAE, 0x42, 0x60, 0x82
};

#endif

