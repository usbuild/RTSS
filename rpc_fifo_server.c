#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <utils.h>
#include <rpc_fifo_server.h>
#include <protocol.h>
#define BUF_LEN 1024

extern int errno;
FILE *server_init(const char *path) {
    int rc = mkfifo(path, S_IRUSR| S_IWUSR);
    if(rc && errno != EEXIST) {
        perror("Create FIFO");
        return NULL;
    }
    return fopen(path, "r");
}

request_t *
parse_args(FILE *f) {
    request_t *rt = (request_t*) malloc(sizeof(request_t));
    char line[BUF_LEN];
    fgets(line, BUF_LEN, f);
    if(line[0] == '=') {
        line[strlen(line + 1) - 2] = '\0';
        rt->argc = atoi(line);
        rt->argv =  (char**)calloc(rt->argc ,sizeof(char*));
        int i = 0;
        for (i = 0; i < rt->argc; ++i) {
            fgets(line, BUF_LEN, f);
            if(line[0] == '~') {
                rt->argv[i] = (char*) calloc(ARG_LEN, sizeof(char));
                line[strlen(line + 1) - 2] = '\0';
                strlcat(rt->argv[i], line + 1);
            }
        }
    }
    return rt;
}

conn_t *
get_connection(FILE *f) {
    request_t *t = parse_args(f);
    if(strcmp(t->argv[0], P_CONNECT) == 0) {
        char *client_fifo_path = strlcat(CLIENT_FIFO, t->argv[1]);
        conn_t *conn = (conn_t*) sizeof(conn_t);
        conn->fd = open(client_fifo_path, "rw");
        return conn;
    }
    return NULL;
}
