//const int MAX_VERTICES = 65534;
//const int MAX_TRIANGLES = 65534;
//const int MAX_GROUPS = 255;
//const int MAX_MATERIALS = 128;
//const int MAX_JOINTS = 128;
//const int MAX_TEXTURE_FILENAME_SIZE = 128;
//
//#define SELECTED		1
//#define HIDDEN			2
//#define SELECTED2		4
//#define DIRTY			8
//#define ISKEY			16
//#define NEWLYCREATED	32
//#define MARKED			64
//
//#define SPHEREMAP		0x80
//#define HASALPHA		0x40
//#define COMBINEALPHA    0x20

enum TransparencyMode
{
	TRANSPARENCY_MODE_SIMPLE,			
	TRANSPARENCY_MODE_DEPTHSORTEDTRIANGLES,
	TRANSPARENCY_MODE_ALPHAREF
};