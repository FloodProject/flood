[vertex]

attribute vec3 vp_Vertex;
attribute vec3 vp_Color;
attribute vec3 vp_MultiTexCoord0;
attribute vec3 vp_Normal;

uniform mat4 vp_ProjectionMatrix;
uniform mat4 vp_ModelViewMatrix;

uniform vec4 vp_LightColors[];
uniform vec3 vp_LightDirection;
uniform mat4 vp_CameraProj;

varying vec3 normal;
varying vec3 lightDirection;
varying vec4 lightColors[];

varying vec4 shadowCoords;
varying vec2 vp_TexCoord;

void main()
{
	normal = vp_Normal;
	lightDirection = vp_LightDirection;
	//lightColors = vp_LightColors;

	shadowCoords = vp_CameraProj * vec4(vp_Vertex, 1.0);

	gl_FrontColor = vec4(vp_Color, 1.0);
	vp_TexCoord = vp_MultiTexCoord0.st;
	gl_Position = vec4(vp_Vertex, 1.0) * vp_ModelViewMatrix * vp_ProjectionMatrix;
} 

[fragment]

uniform sampler2D tex0;
uniform sampler2D vp_ShadowMap;

varying vec2 vp_TexCoord;
varying vec3 normal;
varying vec3 lightDirection;
varying vec4 lightColors[];
varying vec4 shadowCoords;

void main(void)
{
	// Shadow calculation
	vec4 shadowCoordsW = shadowCoords / shadowCoords.w;
	shadowCoordsW.z += 0.0005;
	float distanceFromLight = texture2D(vp_ShadowMap, shadowCoordsW.st).z;

	float shadow = 1.0;
	if (shadowCoords.w > 0.0)
	 		shadow = distanceFromLight < shadowCoordsW.z ? 0.5 : 1.0;
	
	float intensity = dot(lightDirection, normalize(normal));
	vec4 color = vec4( gl_Color.r*intensity, gl_Color.g*intensity, 
		gl_Color.b*intensity, gl_Color.a );

	vec4 tex = texture2D(tex0, vp_TexCoord);
    
    gl_FragColor = tex;//*shadow /**color*/;
}
