/* Copyright (C) 2013 Dominik Ernst

This file is part of Rabenstein.

Rabenstein is free software: you can redistribute it and/or modify it under the
terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
details.

You should have received a copy of the GNU Affero General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>. */

uniform sampler2D bg_tex;
uniform sampler2D level_tex;
uniform sampler2D velocity;

uniform float t1;
uniform float t2;
uniform float t3;
uniform float t4;

void main() {
    vec4 vel = texture2D( velocity, gl_TexCoord[0].st);
    vec4 level = texture2D( level_tex, gl_TexCoord[0].st);

    float dist = 5.0;
    vec2 p1 = gl_TexCoord[0].st - vel.rg * t1 * dist;
    vec2 p2 = gl_TexCoord[0].st - vel.rg * t2 * dist;
    vec2 p3 = gl_TexCoord[0].st - vel.rg * t3 * dist;
    vec2 p4 = gl_TexCoord[0].st - vel.rg * t4 * dist;

    vec4 bg = ( texture2D(bg_tex, p1) * ( 1.0-abs(t1) ) +
                texture2D(bg_tex, p2) * ( 1.0-abs(t2) ) +
                texture2D(bg_tex, p3) * ( 1.0-abs(t3) ) +
                texture2D(bg_tex, p4) * ( 1.0-abs(t4) ) ) /
        ( 1.0-abs(t1) + 1.0-abs(t2) + 1.0-abs(t3) + 1.0-abs(t4) );

    
    gl_FragColor = bg*(1.0f-level.a)  +level *level.a;
}
