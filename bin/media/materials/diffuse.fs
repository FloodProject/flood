uniform sampler2D vp_ShadowMap;
varying float fogFactor;

void main(void)
{
    vec4 shadowSample = texture2D(vp_ShadowMap, vec2(0.0,0.0));
	//gl_FragColor = mix(vec4(0.2, 0.2, 0.2, 1.0), gl_Color, fogFactor );
    gl_FragColor = gl_Color;
}
