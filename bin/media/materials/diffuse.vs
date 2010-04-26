attribute vec3 vp_Vertex;
attribute vec3 vp_Color;

uniform mat4 vp_ProjectionMatrix;
uniform mat4 vp_ModelViewMatrix;

void main()
{
	gl_FrontColor = vec4(vp_Color, 1.0);
	gl_Position = vec4(vp_Vertex, 1.0) * vp_ModelViewMatrix * vp_ProjectionMatrix;
	
	const float LOG2 = 1.442695;
	const float DENSITY = 5.0;
} 
