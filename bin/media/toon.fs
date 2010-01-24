varying vec3 normal;
varying vec3 lightDirection;
varying vec4 lightColors[];

void main(void)
{
	float intensity = dot(lightDirection, normalize(normal));

	vec4 color = vec4( gl_Color.r*intensity, gl_Color.g*intensity, 
		gl_Color.b*intensity, gl_Color.a );

    gl_FragColor = /*gl_Color*/color;
}
