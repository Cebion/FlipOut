FlipOut - Breakout clone with physics
Version 1.1 readme


1. Overview

FlipOut is a breakout game with physics.  Everything is affected by gravity,
and collides with everything else.  Unlike most breakout games, the ball does
not move at a constant speed.  Instead, the paddle can be "flipped" to strike
the ball upwards, much like the flippers in a pinball machine.  Many of the
bonuses affect various aspects of the physics, such as gravity or friction.
The game is fully three-dimensional, and bricks may even be laid in multiple
layers.


2. Controls

The default controls are as follows:

Left   Move paddle left
Right  Move paddle right
Shift  Turbo
Z      Flip paddle counter-clockwise
X      Flip paddle clockwise


3. Configuration

This section documents some of the most useful settings.

Video settings
  This section controls all visual aspects of the game.  If you have a low-end
  graphics card, turning off effects will improve performance.

  Simple models
    Render all bricks as simple boxes.  This can significantly improve perfor-
    mance on low-end systems.

  Vertex buffers
    Enable or disable vertex buffers.  Some ATI cards are known to have problems
    with this enabled.

Game settings
  Brick explosions
    If enabled, bricks will explode into numerous pieces when broken by the
    ball.  This causes significant extra physics load and may bog down older
    systems.

  Optimize physics
    Tries to reduce physics load by detecting inactive objects.  Although it
    works well in most cases, there may be some minor glitches.  Turning this
    off is not recommended unless you have a high-end system.


4. Editor

FlipOut has a built-in level editor.  It's accessible either through the main
menu or the --editor commandline switch.

The dropdowns in the upper left corner can be used to select brick type and
color.  Bricks are placed with the left mouse button and erased with the right
mouse button.  Arrow keys rotate the brick and move it along the Z axis.


5. Hardware requirements

           | Minimum         | Recommended
CPU        | 500 MHz         | 2000 MHz
RAM        | 64 MB           | 128 MB
Video card | 3D accelerated  | GeForce 6 or later


6. Troubleshooting

Problem: The game segfaults on startup.  I am using a Radeon with Mesa DRI
         drivers.

Solution: This a a bug in the driver related to vertex buffers.  Add the
          following line to ~/.flipout/conf to disable them:
          vertex_buffers false;

Problem: Menus work, but the program freezes when trying to start a game.  I am
         using a Radeon on Windows.

Solution: Some ATI cards don't support shaders properly.  Enter the video
          settings section and turn shaders off.

Problem: The menus react to mouse input very sluggishly or not at all.  I am
         using Windows Vista.

Solution: Vista doesn't like something about my code.  Until I figure out what
          it is, try running the game in 32-bit XP compatibility mode.


7. Contact information

The latest releases can be found at http://www.tdb.fi/flipout.shtml

Bug reports, feature requests etc. may be sent to tdb@tdb.fi

Read-only SVN access is available at http://svn.tdb.fi/flipout


8. License

Code and GFX copyright © Mikko Rasa "The DataBeaver"
Music copyright © Esa "e-j"
Grunge font copyright © Nate Piekos, http://www.blambot.com/

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

Full license text can be found in License.txt.


($Id$)
