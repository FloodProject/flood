[vertex]
 
attribute vec3  vp_Vertex;
attribute vec3  vp_TexCoord0;
attribute vec4  vp_Color;
attribute float vp_Normal;

uniform mat4 vp_ModelMatrix;
uniform mat4 vp_ViewMatrix;
uniform mat4 vp_ProjectionMatrix;

varying vec2  v_TexCoord;
varying float v_Shift;

void main()
{
    v_TexCoord = vp_TexCoord0.st;
    v_Shift = vp_Normal;
    
	gl_Position = vp_ProjectionMatrix * vp_ViewMatrix * vp_ModelMatrix * vec4(vp_Vertex, 1.0);
    gl_FrontColor = vp_Color;
}   

[fragment]

uniform sampler2D vp_Texture0;

varying vec2  v_TexCoord;
varying float v_Shift;

void main()
{
    vec4 current = texture2D(vp_Texture0, v_TexCoord);
    vec4 previous= texture2D(vp_Texture0, v_TexCoord+vec2(-1.,0.)/textureSize2D(vp_Texture0,0));
    
    float r = current.r;
    float g = current.g;
    float b = current.b;

    if( v_Shift <= 0.333 )
    {
        float z = v_Shift/0.333;
        r = mix(current.r, previous.b, z);
        g = mix(current.g, current.r,  z);
        b = mix(current.b, current.g,  z);
    } 
    else if( v_Shift <= 0.666 )
    {
        float z = (v_Shift-0.33)/0.333;
        r = mix(previous.b, previous.g, z);
        g = mix(current.r,  previous.b, z);
        b = mix(current.g,  current.r,  z);
    }
   else if( v_Shift < 1.0 )
    {
        float z = (v_Shift-0.66)/0.334;
        r = mix(previous.g, previous.r, z);
        g = mix(previous.b, previous.g, z);
        b = mix(current.r,  previous.b, z);
    }

   float t = max(max(r,g),b);
   vec4 color = vec4(gl_Color.rgb, (r+g+b)/3.0);
   color = t*color + (1.0-t)*vec4(r,g,b, min(min(r,g),b));
   gl_FragColor = vec4( color.rgb, gl_Color.a*color.a);
}
