uniform sampler2D tex0;
uniform sampler2D vp_ShadowMap;
varying vec2 vp_TexCoord;

void main(void)
{
    vec4 shadowSample = texture2D(vp_ShadowMap, vec2(0.0,0.0));
	vec4 color = texture2D(tex0, vp_TexCoord);
    gl_FragColor = color;
}
