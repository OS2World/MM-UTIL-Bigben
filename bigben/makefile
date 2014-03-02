# Makefile fÅr bigben

CC=gcc
CFLAGS=-Wall

all: bigben.exe

bigben.exe: bigben.o
    $(CC) -o bigben.exe bigben.o -lgpp -los2 -s

.cc.o:
    $(CC) $(CFLAGS) -c $*.cc

# Hausputz
clean:
    del *.o

