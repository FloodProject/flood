attribute vec3 vp_Vertex;
attribute vec3 vp_Color;

void main()
{
	gl_FrontColor = vec4(vp_Color, 1.0);

	gl_Position = gl_ModelViewProjectionMatrix * vec4(vp_Vertex, 1.0);
	//ftransform();
} 
