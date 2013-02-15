#include <rpc_fifo_client.h>
#include <config.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
    //conn_t *ct = init_client();
    //
    char buf[1024];
    //int fd = ct->fd;
    int fd = open("/tmp/rtss_client_17198.fifo", O_RDONLY);
    fork();
    while(read(fd, buf, 1) != 0) {
        printf("%ld==>", getpid());
        printf("%s\n", buf);
    }
    return 0;
}
