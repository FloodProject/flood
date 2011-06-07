[vertex]

attribute vec3 vp_Vertex;
attribute vec3 vp_Color;
attribute vec3 vp_Normal;

uniform mat4 vp_ModelMatrix;
uniform mat4 vp_ViewMatrix;
uniform mat4 vp_ProjectionMatrix;

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
	gl_Position = vp_ProjectionMatrix * vp_ViewMatrix * vp_ModelMatrix * vec4(vp_Vertex, 1.0);
}

[fragment]

uniform sampler2D vp_ShadowMap;

varying vec3 normal;
varying vec3 lightDirection;
varying vec4 lightColors[];

void main(void)
{
    vec4 shadowSample = texture2D(vp_ShadowMap, vec2(0.0f,0.0f));
	float intensity = dot(lightDirection, normalize(normal));

	vec4 color = vec4( gl_Color.r*intensity, gl_Color.g*intensity, 
		gl_Color.b*intensity, gl_Color.a );

    gl_FragColor = /*gl_Color*/color;
}
