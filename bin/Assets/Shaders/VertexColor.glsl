[vertex]

attribute vec3 vp_Vertex;
attribute vec4 vp_Color;

uniform mat4 vp_ModelMatrix;
uniform mat4 vp_ViewMatrix;
uniform mat4 vp_ProjectionMatrix;

void main()
{
	gl_FrontColor = vec4(vp_Color);
	gl_Position = vp_ProjectionMatrix * vp_ViewMatrix * vp_ModelMatrix * vec4(vp_Vertex, 1.0);
}

[fragment]

void main(void)
{
    gl_FragColor = gl_Color;
}
