CC=g++
INCLUDES=-I/usr/X11R6/include -I/usr/X11R6/include/X11 
LIBS=-L/usr/X11R6/lib -L/usr/X11R6/lib/X11 -lX11
CFLAGS=-Wall -ggdb

COMPILE=$(CC) $(CFLAGS) $(INCLUDES) $(LIBS)

SRC=common.cpp dispatcher.cpp chan.cpp player.cpp vlc.cpp main.cpp
ELF=dumbmediacenter

all: 
	$(COMPILE) $(SRC) -o $(ELF)
