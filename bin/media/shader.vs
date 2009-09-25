attribute vec3 vp_Vertex;
attribute vec3 vp_Color;
attribute vec3 vp_MultiTexCoord0;

void main()
{
	gl_FrontColor = vec4(vp_Color, 1.0);
	gl_TexCoord[0] = vec4(vp_MultiTexCoord0, 1.0);

	gl_Position = gl_ModelViewProjectionMatrix * vec4(vp_Vertex, 1.0);
} 
