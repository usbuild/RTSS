#include <rpc_fifo_client.h>
#include <service.h>
#include <malloc.h>
#include <protocol.h>
#include <config.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
    conn_t *ct = init_client();
    if(login("abc", "12345", ct) == 0) {
        puts("LOGGED!");
    } else {
        puts("ERROR!");
    }
    
    t_ticket_list *list = query_ticket("hai", "jing", ct);
    int i;
    for (i = 0; i < list->num; ++i) {
        puts(list->data[i].id);
    }

    release_connection(ct);
    return 0;
}
