uniform sampler2D bg_tex;
uniform sampler2D level_tex;
uniform sampler2D velocity;

uniform int frame_counter;

void main()
{
  
    vec4 vel = texture2D( velocity, gl_TexCoord[0].st);
    vec4 level = texture2D( level_tex, gl_TexCoord[0].st);

    // float mag = sqrt(vel.r*vel.r+vel.g*vel.g)/1.0;
    
    
    float t1 = (( int(frame_counter)     ) % 100)/50.0-1.0f;
    float t2 = (( int(frame_counter) + 25) % 100)/50.0-1.0f;
    float t3 = (( int(frame_counter) + 50) % 100)/50.0-1.0f;
    float t4 = (( int(frame_counter) + 75) % 100)/50.0-1.0f;
        
    //vel /= mag;

    vec2 p1 = gl_TexCoord[0].st*0.9+0.05  - vel.rg * t1 * 0.4;
    vec2 p2 = gl_TexCoord[0].st*0.9+0.05 - vel.rg * t2 * 0.4;
    vec2 p3 = gl_TexCoord[0].st*0.9+0.05 - vel.rg * t3 * 0.4;
    vec2 p4 = gl_TexCoord[0].st*0.9+0.05 - vel.rg * t4 * 0.4;
       
    vec4 bg =( texture2D(bg_tex, p1) * ( 1.0-abs(t1) ) +
               texture2D(bg_tex, p2) * ( 1.0-abs(t2) ) +
               texture2D(bg_tex, p3) * ( 1.0-abs(t3) ) +
               texture2D(bg_tex, p4) * ( 1.0-abs(t4) ) ) /
        ( 1.0-abs(t1) + 1.0-abs(t2) + 1.0-abs(t3) + 1.0-abs(t4) );
 

    gl_FragColor = bg*(1.0f-level.a)  +level *level.a;


}
