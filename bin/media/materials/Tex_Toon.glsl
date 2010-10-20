[vertex]

#version 110

attribute vec3 vp_Vertex;
attribute vec3 vp_Color;
attribute vec3 vp_TexCoord0;
attribute vec3 vp_Normal;

uniform mat4 vp_ModelMatrix;
uniform mat4 vp_ViewMatrix;
uniform mat4 vp_ProjectionMatrix;

varying vec3 normal;
varying vec2 texCoord0;

void main()
{
	normal = vp_Normal; // * mat3(vp_ModelMatrix);

	gl_FrontColor = vec4(vp_Color, 1.0);
	texCoord0 = vp_TexCoord0.st;
	gl_Position = vec4(vp_Vertex, 1.0) * vp_ModelMatrix * vp_ViewMatrix * vp_ProjectionMatrix;
} 

[fragment]

#version 110

uniform vec4 vp_LightColors[];
uniform vec3 vp_LightDirection;
uniform sampler2D vp_Texture0;

varying vec2 texCoord0;
varying vec3 normal;

void main(void)
{
	vec4 ambient = vec4(0.3,0.3,0.25,1.0);
	
	float diffuse = 0.6;
	//float diffuse = dot(vec3(0.5,0.5,0.5), normal);
	//diffuse = pow((0.5*diffuse)+0.5,2.0);
	
	vec4 tex = texture2D(vp_Texture0, texCoord0);
	vec4 albedo = tex*diffuse;
	
	gl_FragColor = (tex*ambient)+albedo;
}
