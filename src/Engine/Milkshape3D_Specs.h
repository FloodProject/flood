/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

namespace vapor {

//-----------------------------------//

static const int MAX_VERTICES				= 65534;
static const int MAX_TRIANGLES				= 65534;
static const int MAX_GROUPS					= 255;
static const int MAX_MATERIALS				= 128;
static const int MAX_JOINTS					= 128;
static const int MAX_TEXTURE_FILENAME_SIZE	= 128;

static const int SELECTED		= 1;
static const int HIDDEN			= 2;
static const int SELECTED2		= 4;
static const int DIRTY			= 8;
static const int ISKEY			= 16;
static const int NEWLYCREATED	= 32;
static const int MARKED			= 64;

static const int SPHEREMAP		= 0x80;
static const int HASALPHA		= 0x40;
static const int COMBINEALPHA	= 0x20;

enum TransparencyMode
{
	TRANSPARENCY_MODE_SIMPLE,			
	TRANSPARENCY_MODE_DEPTHSORTEDTRIANGLES,
	TRANSPARENCY_MODE_ALPHAREF
};

//-----------------------------------//

#ifdef VAPOR_COMPILER_MSVC
	#pragma pack( push, 1 )
#endif

struct VAPOR_ALIGN_BEGIN(1) ms3d_header_t
{
	char    id[10];
	long    version;
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_vertex_t
{
	byte	flags;
	Vector3	position;
	char	boneIndex;
	byte	referenceCount;
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_triangle_t
{
	ushort	flags;
	ushort	vertexIndices[3];
	Vector3	vertexNormals[3];
	float	s[3];
	float	t[3];
	byte	smoothingGroup;
	byte	groupIndex;
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_group_t
{
	byte flags;
	char name[32];
	ushort numGroupTriangles;
	std::vector<ushort> triangleIndices;
	char materialIndex;
	char comment[32];
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_material_t
{
	char			name[32];
	Color			ambient;
	Color			diffuse;
	Color			specular;
	Color			emissive;
	float			shininess;
    float			transparency;
	byte			mode;
	char			texture[MAX_TEXTURE_FILENAME_SIZE];
    char			alphamap[MAX_TEXTURE_FILENAME_SIZE];
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_keyframe_t
{
	float time;
	Vector3 parameter;
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_tangent_t
{
	Vector3 tangentIn;
	Vector3 tangentOut;
} VAPOR_ALIGN_END(1);

//-----------------------------------//

struct VAPOR_ALIGN_BEGIN(1) ms3d_joint_t
{
	byte flags;
	char name[32];
	char parentName[32];
	int indexParent;

	EulerAngles rotation;
	Vector3 position;

	std::vector<ms3d_keyframe_t> rotationKeys;
	std::vector<ms3d_keyframe_t> positionKeys;
	std::vector<ms3d_tangent_t> tangents;

	Matrix4x3 relativeMatrix;
	Matrix4x3 absoluteMatrix;

	//std::vector<char> comment;
	//float color[3];
} VAPOR_ALIGN_END(1);

#ifdef VAPOR_COMPILER_MSVC
	#pragma pack( pop )
#endif

//-----------------------------------//

} // end namespace