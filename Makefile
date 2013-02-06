CC=gcc
GTK_INC=$(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS=$(shell pkg-config --libs gtk+-3.0)
LIBS = -lm
CFLAGS = -g -Wl,--export-dynamic
CFLAGS += $(GTK_INC)
LIBS += $(GTK_LIBS)

all:client server

client:client.c
	$(CC) $^ $(CFLAGS) -o $@  $(LIBS)

server:server.o

clean:
	-rm *.o client server
