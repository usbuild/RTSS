#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <dbutils.h>
#include <user.h>
#include <ticket.h>
#include <config.h>
#include <rpc_fifo_server.h>

int main(int argc, const char *argv[])
{
    FILE *f = server_init(SERVER_FIFO);
    conn_t *t = get_connection(f);
    if(t != NULL) {
        printf("%d\n", t->fd);
    }
    int i;
    char a = 'a';
    for (i = 0; i < 10; ++i) {
        write(t->fd, &a, 1, 1);
    }
    return 0;
}
