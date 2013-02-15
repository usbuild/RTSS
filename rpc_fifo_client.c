#include <rpc_fifo_client.h>
#include <stdio.h>
#include <stdlib.h>
#include <protocol.h>
#include <string.h>
#include <utils.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#define BUF_LEN 1024

extern int errno;
conn_t *init_client() {
    pid_t pid = getpid();
    request_t rqst;
    rqst.argc = 2;
    rqst.argv = (char**) malloc(sizeof(char*) * rqst.argc);
    rqst.argv[0] = P_CONNECT;
    char *tmp = ltoa(pid);
    rqst.argv[1] = tmp;
    char *str = build_request_str(&rqst);
    free(rqst.argv);
    conn_t *ct = (conn_t*) malloc(sizeof(conn_t));
    char *t = strlcat(CLIENT_FIFO, tmp);
    char *path = strlcat(t, ".fifo");
    free(tmp);
    free(t);
    int rc = mkfifo(path, S_IRUSR | S_IWUSR);
    if(rc && errno != EEXIST) {
        perror("Create FIFO");
        return NULL;
    }
    ct->fd = open(path, O_RDWR);
    
    rc = open(SERVER_FIFO, O_WRONLY);
    if(errno) {
        perror("Connect Server");
        exit(1);
    }
    puts(path);
    write(rc, str, strlen(str));
    free(str);
    return ct;
}

char *build_request_str(request_t *rqst) {
    char *str = (char*)malloc(sizeof(char) * 1024);
    char *tmp = ltoa(rqst->argc);
    strcpy(str, "=");
    strcat(str, tmp);
    strcat(str, "\r\n");
    free(tmp);
    int i;
    for (i = 0; i < rqst->argc; ++i) {
        strcat(str, "~");
        strcat(str, rqst->argv[i]);
        strcat(str, "\r\n");
    }
    return str;
}
