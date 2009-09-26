attribute vec3 vp_Vertex;
attribute vec3 vp_Color;
attribute vec3 vp_MultiTexCoord0;

varying vec2 vp_TexCoord;

void main()
{
	gl_FrontColor = vec4(vp_Color, 1.0);
	vp_TexCoord = vp_MultiTexCoord0.st;

	gl_Position = gl_ModelViewProjectionMatrix * vec4(vp_Vertex, 1.0);
} 
