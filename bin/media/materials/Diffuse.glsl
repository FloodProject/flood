[vertex]

attribute vec3 vp_Vertex;
attribute vec3 vp_Color;

uniform mat4 vp_ProjectionMatrix;
uniform mat4 vp_ModelViewMatrix;

void main()
{
	gl_FrontColor = vec4(vp_Color, 1.0);
	gl_Position = vec4(vp_Vertex, 1.0) * vp_ModelViewMatrix * vp_ProjectionMatrix;
}

[fragment]
//uniform sampler2D vp_ShadowMap;

void main(void)
{
    //vec4 shadowSample = texture2D(vp_ShadowMap, vec2(0.0,0.0));
	//gl_FragColor = mix(vec4(0.2, 0.2, 0.2, 1.0), gl_Color, fogFactor );
    gl_FragColor = gl_Color;
}
