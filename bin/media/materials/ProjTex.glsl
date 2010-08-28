[vertex]

attribute vec3 vp_Vertex;
attribute vec3 vp_TexCoord0;

uniform mat4 vp_ModelMatrix;
uniform mat4 vp_ViewMatrix;
uniform mat4 vp_ProjectionMatrix;
uniform mat4 vp_ProjectionTextureMatrix;

varying vec2  vp_TexCoord;

void main()
{
	vp_TexCoord = vp_TexCoord0.st;
	gl_Position = vec4(vp_Vertex, 1.0) * vp_ModelMatrix * vp_ViewMatrix * vp_ProjectionMatrix;
} 

[fragment]

uniform sampler2D vp_Texture0;
varying vec2 vp_TexCoord;

void main(void)
{
	vec4 color = texture2D(vp_Texture0, vp_TexCoord);
	gl_FragColor = color;
}
