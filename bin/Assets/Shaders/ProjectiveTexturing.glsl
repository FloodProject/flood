[vertex]

attribute vec3 vp_Vertex;
attribute vec3 vp_TexCoord0;

uniform mat4 vp_ModelMatrix;
uniform mat4 vp_ViewMatrix;
uniform mat4 vp_ProjectionMatrix;

uniform mat4 vp_TextureProjection;
uniform mat4 vp_TextureView;

//varying vec2  vp_TexCoord;
varying vec4  vp_ProjTexCoord;

void main()
{
	gl_Position = vp_ProjectionMatrix * vp_ViewMatrix * vp_ModelMatrix * vec4(vp_Vertex, 1.0);
	vp_ProjTexCoord = vp_TextureProjection * vp_TextureView * vp_ModelMatrix * vec4(vp_Vertex, 1.0);
	
}

[fragment]

uniform sampler2D vp_Texture0;
//varying vec2 vp_TexCoord;
varying vec4  vp_ProjTexCoord;

void main(void)
{
	vec2 projCoords = (vp_ProjTexCoord.st / vp_ProjTexCoord.q)*0.5 + 0.5;
	vec4 color = texture2D(vp_Texture0, projCoords);
	gl_FragColor = color; //vec4(1,1, 0, 1);
}
