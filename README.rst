
Houdini Particle Viewer
=======================

Summary
-------

This is a Houdini OpenGL rendering hook designed to enhance the display of
particles in the viewport. 

It is limited to Cube and Disc rendering. Further display types are planned.

Details
-------

It loads by default if it is on the HOUDINI_DSO_PATH and then you enable it with
viewport settings.

Go to the viewport, press d, go to drop down by custom and add a new generic
attribute with name "particleviewerbox" and label as something nicer "Particle
Viewer Box" and make sure it is toggled on and the boxes should appear in the
view port. 

Modes
~~~~~

Two modes are available:

**particleviewerbox**
   Draws a box around each particle. The box is affected by "scale", "rotate"
   and "Cd" attributes on each particle.

**particleviewerdisc**
   Draws a disc at each particle position. The disc is affected by "scale", "rotate"
   and "Cd" attributes on each particle. If the "rotate" attribute is not
   present, the disc will be drawn facing the camera.

Notes
-----

The code is compiled and tested on an Ubuntu 10.4 system with g++-4.1 installed.
It runs on Houdini 11.0.581 Apprentice build. In the "About Houdini" dialog at
states "Platform: linux-x86_64-gcc4.1".

I've have used this project to experiment with apenwarr's "redo" build system.
Creating a Makefile equivalent would not be hard, the trouble is the Houdini
install specific details in each target setup.





