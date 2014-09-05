CFLAGS=-Wall -pedantic -std=c99 -g
CFLAGS+=`pkg-config gdk-pixbuf-2.0 --cflags`
LDFLAGS=-lGL -lGLU -lglut -lm `pkg-config gdk-pixbuf-2.0 --libs` -lalut -lopenal 
 
ASSIGNMENT2=assignment
ASSIGNMENT2_SOURCES=texture.c main.c geometry.c controls.c lighting.c colors.c util.c projectile.c sound.c
ASSIGNMENT2_HEADERS=texture.h main.h geometry.h controls.h lighting.h colors.h util.h projectile.h sound.h

all: $(ASSIGNMENT2)

$(ASSIGNMENT2): $(ASSIGNMENT2_SOURCES) $(ASSIGNMENT2_HEADERS)
	gcc -o $@ $(ASSIGNMENT2_SOURCES) $(CFLAGS) $(LDFLAGS)

clean: 
	rm -f $(ASSIGNMENT2) *~ *.o
