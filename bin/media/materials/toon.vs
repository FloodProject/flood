attribute vec3 vp_Vertex;
attribute vec3 vp_Color;
attribute vec3 vp_Normal;

uniform mat4 vp_ProjectionMatrix;
uniform mat4 vp_ModelMatrix;
uniform mat4 vp_ViewMatrix;
uniform mat4 vp_ModelViewMatrix;

uniform vec4 vp_LightColors[];
uniform vec3 vp_LightDirection;

varying vec3 normal;
varying vec3 lightDirection;
varying vec4 lightColors[];

void main()
{
	normal = vp_Normal;
	lightDirection = vp_LightDirection;
	//lightColors = vp_LightColors;

	gl_FrontColor = vec4(vp_Color, 1.0);
	gl_Position = vec4(vp_Vertex, 1.0) * vp_ModelViewMatrix * vp_ProjectionMatrix;
}