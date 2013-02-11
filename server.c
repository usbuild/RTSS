#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <dbutils.h>
#include <user.h>
#include <ticket.h>
#include <config.h>
#include <rpc_fifo_server.h>

void init() { // init the system
    db_init();
}



int main(int argc, const char *argv[])
{
    init();
    //start_server(SERVER_FIFO);
    return 0;
}
