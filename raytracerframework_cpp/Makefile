############################################################################
# Makefile for the raytracer framwork created for the Computer Science 
# course "Introduction Computer Graphics" taught at the University of 
# Groningen by Tobias Isenberg.
############################################################################

### MACROS

# GNU (everywhere)
CPP = g++ -std=c++17 -Xpreprocessor -fopenmp -g -Wall -D_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR

# GNU (faster)
#CPP = g++ -O5 -Wall -fomit-frame-pointer -ffast-math 

#LIBS = -lm  -fopenmp

#MacOSX specific libs
LIBS = -lm -lomp

EXECUTABLE = ../ray

OBJS = main.o raytracer.o sphere.o light.o material.o \
	   image.o triple.o lodepng.o scene.o torus.o deg4solver.o \
	   object.o rMatrix.o cone.o camera.o triangle.o disc.o plane.o \
	   csg.o mesh.o glm.o

YAMLOBJS = $(subst .cpp,.o,$(wildcard yaml/*.cpp))

IMAGES = $(subst .yaml,.png,$(wildcard *.yaml))


### TARGETS

$(EXECUTABLE): $(OBJS) $(YAMLOBJS)
	$(CPP) $(OBJS) $(YAMLOBJS) $(LIBS) -o $@

run: $(IMAGES)

%.png: %.yaml $(EXECUTABLE)
	./$(EXECUTABLE) $<

depend: make.dep

clean:
	- /bin/rm -f  *.bak *~ $(OBJS) $(YAMLOBJS) $(EXECUTABLE) $(EXECUTABLE).exe

make.dep:
	gcc -MM $(OBJS:.o=.cpp) > make.dep

### RULES

.SUFFIXES: .cpp .o .yaml .png

.cpp.o:
	$(CPP) -c -o $@ $<

### DEPENDENCIES

include make.dep
