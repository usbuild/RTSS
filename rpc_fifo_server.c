#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
    return fopen(path, "r+");
}

request_t *
parse_args(FILE *f) {
    request_t *rt = (request_t*) malloc(sizeof(request_t));
    char line[BUF_LEN];
    fgets(line, BUF_LEN, f);
    if(line[0] == '=') {
        line[strlen(line) - 2] = 0;
        rt->argc = atoi(line + 1);
        rt->argv =  (char**)calloc(rt->argc ,sizeof(char*));
        int i = 0;
        for (i = 0; i < rt->argc; ++i) {
            fgets(line, BUF_LEN, f);
            if(line[0] == '~') {
                rt->argv[i] = (char*) calloc(strlen(line), sizeof(char));
                line[strlen(line) - 2] = 0;
                strcpy(rt->argv[i], line + 1);
            }
        }
    }
    return rt;
}

conn_t *
get_connection(FILE *f) {
    request_t *t = parse_args(f);
    if(strcmp(t->argv[0], P_CONNECT) == 0) {
        char *tmp = strlcat(CLIENT_FIFO, t->argv[1]);
        char *client_fifo_path_c = strlcat(tmp, "_c.fifo");
        char *client_fifo_path_d = strlcat(tmp, "_d.fifo");
        free(tmp);

        conn_t *conn = (conn_t*) malloc(sizeof(conn_t));
        conn->cfd = open(client_fifo_path_c, O_RDONLY);
        conn->dfd = open(client_fifo_path_d, O_WRONLY);
        conn->input = fdopen(conn->cfd, "r");
        return conn;
    }
    return NULL;
}

void
release_conn(conn_t *c) {
    fclose(c->input);
    close(c->cfd);
    close(c->dfd);
}
