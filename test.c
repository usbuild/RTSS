#include <rpc_fifo_client.h>
#include <config.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
    conn_t *ct = init_client();
    char buf[1024];
    int fd = ct->dfd;
    while(read(fd, buf, 1) != 0) {
        printf("%s\n", buf);
    }
    release_connection(ct);
    return 0;
}
