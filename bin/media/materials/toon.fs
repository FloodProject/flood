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
