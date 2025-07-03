# Final Project
EXE=final

# Main target
all: $(EXE)

#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f -r *.exe *.o *.a util/*.o util/*.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a util/*.o util/*.a
endif

# Dependencies
final.o: final.c util/CSCIx229.h satellite.h
swordfish2.o: swordfish2.c util/CSCIx229.h swordfish2.h
satellite.o: satellite.c util/CSCIx229.h satellite.h
util/fatal.o: util/fatal.c util/CSCIx229.h
	gcc -c $(CFLG) -o $@ $<
util/errcheck.o: util/errcheck.c util/CSCIx229.h
	gcc -c $(CFLG) -o $@ $<
util/print.o: util/print.c util/CSCIx229.h
	gcc -c $(CFLG) -o $@ $<
util/loadtexbmp.o: util/loadtexbmp.c util/CSCIx229.h
	gcc -c $(CFLG) -o $@ $<
util/loadobj.o: util/loadobj.c util/CSCIx229.h
	gcc -c $(CFLG) -o $@ $<
util/projection.o: util/projection.c util/CSCIx229.h
	gcc -c $(CFLG) -o $@ $<

#  Create archive
util/CSCIx229.a:util/fatal.o util/errcheck.o util/print.o util/loadtexbmp.o util/loadobj.o util/projection.o
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG)  $<
.cpp.o:
	g++ -c $(CFLG)  $<

#  Link
final:final.o util/CSCIx229.a satellite.o swordfish2.o
	gcc $(CFLG) -o $@ $^  $(LIBS)

#  Clean
clean:
	$(CLEAN)
