uniform vec4 ucolor;

void main (void)
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0].xyz = gl_MultiTexCoord0.xyz;
    gl_FrontColor = vec4(ucolor.rgb, gl_Color.a);
}
