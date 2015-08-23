/*
    This file is part of GlakkeGame.

    GlakkeGame, a 2D game.
    Copyright (C) 2010  Andrée Ekroth

    GlakkeGame is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GlakkeGame is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GlakkeGame.  If not, see <http://www.gnu.org/licenses/>.
*/

uniform sampler2D tex;

void main()
{
//     vec4 color = vec4(gl_Color.x, gl_Color.y, gl_Color.z, texture2D(tex, gl_TexCoord[0].st).x * gl_Color.w);
    vec4 color = vec4(1, 1, 1, texture2D(tex, gl_TexCoord[0].st).x);
    gl_FragColor = color;
}
