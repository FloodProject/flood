varying float fogFactor; 

void main(void)
{
	gl_FragColor = mix(vec4(0.2, 0.2, 0.2, 1.0), gl_Color, fogFactor );
    //gl_FragColor = gl_Color;
}
