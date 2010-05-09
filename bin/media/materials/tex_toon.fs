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
