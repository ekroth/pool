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

// attribute vec2 position;
// attribute vec2 texcoord;

void main()
{
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
}