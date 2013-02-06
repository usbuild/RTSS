CC=gcc
CFLAGS=-lncurses -lmenu -lpanel -lm -lform

all:client server

client:client.o

server:server.o

clean:
	-rm *.o client server
