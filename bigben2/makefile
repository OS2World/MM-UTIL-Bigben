# Makefile for bigben2
# using emx-0.8g

CC=gcc
CFLAGS=-Wall -Zomf -O
LFLAGS=-Zomf -Zsys -s

all: bigben2.exe

bigben2.exe: bigben2.obj bigben2.def
    $(CC) -o bigben2.exe bigben2.obj bigben2.def $(LFLAGS)

bigben2.obj: bigben2.cc
    $(CC) $(CFLAGS) -c bigben2.cc

.cc.obj:
    $(CC) $(CFLAGS) -c $*.cc

# clean up
clean:
    del *.obj

