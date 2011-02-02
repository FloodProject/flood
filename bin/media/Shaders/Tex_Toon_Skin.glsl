[vertex]

#version 110

attribute vec3 vp_Vertex;
attribute vec3 vp_Color;
attribute vec3 vp_TexCoord0;
attribute vec3 vp_Normal;
attribute float vp_BoneIndex;

uniform mat4 vp_ModelMatrix;
uniform mat4 vp_ViewMatrix;
uniform mat4 vp_ProjectionMatrix;
uniform mat4 vp_BonesMatrix[50];

varying vec3 normal;
varying vec2 vp_TexCoord;

void main()
{
	normal = vp_Normal;// * mat3(vp_BonesMatrix[vp_BoneIndex]) * mat3(vp_ModelMatrix);
	//normal = normalize(normal);
	
	vp_TexCoord = vp_TexCoord0.st;
	gl_Position = vp_ProjectionMatrix * vp_ViewMatrix * vp_ModelMatrix
	 * vp_BonesMatrix[int(vp_BoneIndex)] * vec4(vp_Vertex, 1.0);
} 

[fragment]

uniform vec4 vp_LightColors[];
uniform vec3 vp_LightDirection;
uniform sampler2D vp_Texture0;

varying vec2 vp_TexCoord;
varying vec3 normal;

void main(void)
{
	vec4 ambient = vec4(0.3,0.3,0.25,1.0);

	float diffuse = dot(vec3(0.5,0.5,0.5), normal);
	//diffuse = pow((0.5*diffuse)+0.5,2);
	
	vec4 tex = texture2D(vp_Texture0, vp_TexCoord);
	vec4 albedo = tex*diffuse;
	
	gl_FragColor = (tex*ambient)+albedo;
}
