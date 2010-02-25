varying float fogFactor; 

void main(void)
{
	gl_FragColor = mix(vec4(0.5, 0.5, 0.5, 1.0), gl_Color, fogFactor );
    //gl_FragColor = gl_Color;
}
