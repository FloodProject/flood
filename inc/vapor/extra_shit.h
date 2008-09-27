typedef unsigned char	byte;
typedef unsigned short	ushort;
typedef unsigned int	uint;

#define MAX_VERTICES				65534
#define MAX_TRIANGLES				65534
#define MAX_GROUPS					255
#define MAX_MATERIALS				128
#define MAX_JOINTS					128
#define MAX_TEXTURE_FILENAME_SIZE	128

#include <pshpack1.h>

struct ms3d_header_t
{
	char    id[10];
	long    version;
};

struct ms3d_vertex_t
{
	byte	flags;
	float	vertex[3];
	char	boneId;
	byte referenceCount;
	char boneIds[3];
	//unsigned char weights[3];
	//unsigned int extra;
	//float renderColor[3];
};

struct ms3d_triangle_t
{
	ushort	flags;
	ushort	vertexIndices[3];
	float	vertexNormals[3][3];
	float	s[3];
	float	t[3];
	float	normal[3];
	byte	smoothingGroup;
	byte	groupIndex;
};

struct ms3d_group_t
{
	unsigned char flags;
	char name[32];
	std::vector<unsigned short> triangleIndices;
	char materialIndex;
	std::vector<char> comment;
};

struct ms3d_material_t
{
	char			name[32];
	float			ambient[4];
	float			diffuse[4];
	float			specular[4];
	float			emissive[4];
	float			shininess;
    float			transparency;
	byte			mode;
	char			texture[MAX_TEXTURE_FILENAME_SIZE];
    char			alphamap[MAX_TEXTURE_FILENAME_SIZE];
	byte			id;
	std::vector<char>	comment;
};

#include <poppack.h>




































//
//enum TransparencyMode
//{
//	TRANSPARENCY_MODE_SIMPLE,			
//	TRANSPARENCY_MODE_DEPTHSORTEDTRIANGLES,
//	TRANSPARENCY_MODE_ALPHAREF
//};

	//float m_jointSize;
	//int m_transparencyMode;
	//float m_alphaRef;


	//float m_animationFps;
	//float m_currentTime;
	//int m_totalFrames;

	//vector<ms3d_joint_t> m_joints;


	//int GetNumJoints() const;
	//ms3d_joint_t *GetJoint(int index);

	//float GetJointSize() const;
	//int GetTransparencyMode() const;
	//float GetAlphaRef() const;

	//int FindJointByName(const char *name);
	//void SetupJoints();
	//void SetupTangents();
	//void SetFrame(float frame);
	//void EvaluateJoint(int index, float frame);

	//float GetAnimationFps() const;
	//float GetCurrentFrame() const;
	//int GetTotalFrames() const;

