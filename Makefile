CC=gcc

GTK_INC=$(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS=$(shell pkg-config --libs gtk+-3.0)

SQLITE_INC=$(shell pkg-config --cflags sqlite3)
SQLITE_LIBS=$(shell pkg-config --libs sqlite3)

LIBS =
CFLAGS = -g -Wl,--export-dynamic -I.

.PHONY:all clean
all: server client

client: CFLAGS += $(GTK_INC)
client: LIBS += $(GTK_LIBS)
server: CFLAGS += $(SQLITE_INC)
server: LIBS += $(SQLITE_LIBS)
client:client.c rpc_client.c utils.c service.c
	$(CC) $^ $(CFLAGS) -o $@  $(LIBS)

server:server.c dbutils.c user.c ticket.c utils.c rpc_server.c provider.c station.c log.c
	$(CC) $^ $(CFLAGS) -o $@  $(LIBS)

clean:
	-rm client server
