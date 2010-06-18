[vertex]

attribute vec3 vp_Vertex;
attribute vec3 vp_Color;
attribute vec3 vp_MultiTexCoord0;
attribute vec3 vp_Normal;

uniform mat4 vp_ProjectionMatrix;
uniform mat4 vp_ModelViewMatrix;

uniform mat4 vp_CameraProj;

varying vec3 normal;

varying vec4 shadowCoords;
varying vec2 vp_TexCoord;

void main()
{
	normal = vp_Normal;

	gl_FrontColor = vec4(vp_Color, 1.0);
	vp_TexCoord = vp_MultiTexCoord0.st;
	gl_Position = vec4(vp_Vertex, 1.0) * vp_ModelViewMatrix * vp_ProjectionMatrix;
} 

[fragment]

uniform vec4 vp_LightColors[];
uniform vec3 vp_LightDirection;
uniform sampler2D tex0;

varying vec2 vp_TexCoord;
varying vec3 normal;

void main(void)
{
	float intensity = dot(vec3(0,1,0), normal);

	vec4 tex = texture2D(tex0, vp_TexCoord);
	gl_FragColor = tex*intensity; //vec4(normal,1.0); //
}
