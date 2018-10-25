#changing platform dependant stuff, do not change this
# Linux (default)
LDFLAGS = -lGL -lGLU -lglut
CFLAGS=-g -Wall -std=c++11
CC=g++
EXEEXT=
RM=rm

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXEEXT=.exe #on windows applications must have .exe extension
	RM=del #rm command for windows powershell
    LDFLAGS = -lfreeglut -lglu32 -lopengl32
else
	# OS X
	OS := $(shell uname)
	ifeq ($(OS), Darwin)
	        LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT -std=gnu++11
	endif
endif

PROGRAM_NAME= Modeller.x

run: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)$(EXEEXT)

$(PROGRAM_NAME): Modeller.o math3D.o SceneGraph.o Node.o NodeTransform.o NodeObject.o Material.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) *.o $(PROGRAM_NAME)$(EXEEXT)