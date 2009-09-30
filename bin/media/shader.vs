attribute vec3 vp_Vertex;
attribute vec3 vp_Color;
attribute vec3 vp_MultiTexCoord0;

uniform mat4 vp_Projection;
uniform mat4 vp_ModelMatrix;
uniform mat4 vp_ViewMatrix;
uniform mat4 vp_ModelViewMatrix;

varying vec2 vp_TexCoord;

void main()
{
	gl_FrontColor = vec4(vp_Color, 1.0);
	vp_TexCoord = vp_MultiTexCoord0.st;

	gl_Position = gl_ProjectionMatrix * vp_ModelViewMatrix * vec4(vp_Vertex, 1.0);
} 
