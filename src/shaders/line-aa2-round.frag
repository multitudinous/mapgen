void main(void)
{
    vec4 color = gl_Color;
    vec3  p = gl_TexCoord[0].xyz;
    float dist;

    if( p.z < 0.0 )
    {
        float xdist = min(abs(p.x),abs(p.x-1.0));
        float xwidth = fwidth(xdist);
        float x = smoothstep(1.0-xwidth, 1.0+xwidth, xdist);

        float ydist = abs(p.y);
        float ywidth = fwidth(ydist);
        float y = smoothstep(1.0-ywidth, 1.0+ywidth, ydist);

        float alpha = 1.0-max(x,y);
        gl_FragColor = vec4(color.rgb, color.a*alpha);
        return;
    }

    if( (p.x < 0.0) )
        dist = sqrt(p.x*p.x+p.y*p.y);
    else if( (p.x > 1.0) )
        dist = sqrt((p.x-1.0)*(p.x-1.0)+p.y*p.y);
    else
        dist = abs(p.y);
    float width = fwidth(dist);
    if( abs(p.z) <= 1.0) // alpha coverage
        width = dist;
    float alpha = smoothstep(1.0+width,1.0-width, dist);

    gl_FragColor = vec4(color.rgb, color.a*alpha);
}
