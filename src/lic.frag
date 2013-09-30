uniform sampler2D bg_tex;
uniform sampler2D level_tex;
uniform sampler3D velocity;

uniform int frame_counter;

void main()
{
    vec2 warped_coordinates = gl_TexCoord[0].st;

    warped_coordinates.x += sin(float(frame_counter) / 10.0f + gl_TexCoord[0].y * 110.0f) / 700.0f;
    warped_coordinates.y += cos(float(frame_counter) / 10.0f + gl_TexCoord[0].x * 110.0f) / 700.0f;

    vec4 level = texture2D(level_tex, gl_TexCoord[0].st);
    vec4 bg = texture2D(bg_tex, warped_coordinates);

    gl_FragColor =  level * level.a + bg * (1.0f - level.a);
}
