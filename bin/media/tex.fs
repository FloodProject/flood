uniform sampler2D tex0;
varying vec2 vp_TexCoord;

void main(void)
{
	vec4 color = texture2D(tex0, vp_TexCoord);
    gl_FragColor = color;
}
