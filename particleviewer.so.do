DEPS="src/GR_particleviewer.o src/Renderer.o src/DisplayObject.o src/plugin.o"

redo-ifchange $DEPS

g++-4.1 -o $3 -shared $DEPS -L/usr/X11R6/lib64 -L/usr/X11R6/lib -lGLU -lGL -lX11 -lXext -lXi -ldl

