CC=gcc

GTK_INC=$(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS=$(shell pkg-config --libs gtk+-3.0)

SQLITE_INC=$(shell pkg-config --cflags sqlite3)
SQLITE_LIBS=$(shell pkg-config --libs sqlite3)
LIBS = -lm

CFLAGS = -g -Wl,--export-dynamic -I.
#CFLAGS += $(GTK_INC)
CFLAGS += $(SQLITE_INC)

#LIBS += $(GTK_LIBS)
LIBS += $(SQLITE_LIBS)

.PHONY:all clean
all: server client

client:test.c rpc_fifo_client.c utils.c service.c
	$(CC) $^ $(CFLAGS) -o $@  $(LIBS)

server:server.c dbutils.c user.c ticket.c utils.c rpc_fifo_server.c provider.c
	$(CC) $^ $(CFLAGS) -o $@  $(LIBS)

clean:
	-rm *.o client server
