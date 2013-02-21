#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <provider.h>
#include <unistd.h>
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

    signal(SIGCHLD, SIG_IGN);
    db_init();
    FILE *f = server_init(SERVER_FIFO);
    pid_t ppid = getpid();
    conn_t *client_conn = NULL;
    while(getpid() == ppid) {
        if(client_conn != NULL) release_conn(client_conn);
        client_conn = get_connection(f);
        fork();
    }
    if(client_conn != NULL) {
        handle_client(client_conn);
    }
    return 0;
}
