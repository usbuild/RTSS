#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <utils.h>
#include <rpc_server.h>
#include <protocol.h>
#ifdef REMOTE_VERSION
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#define BUF_LEN 1024

extern int errno;
#ifdef LOCAL_VERSION
FILE *server_init(const char *path) {
    int rc = mkfifo(path, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP);
    if(rc && errno != EEXIST) {
        perror("Create FIFO");
        return NULL;
    }
    return fopen(path, "r+");
}
#endif

#ifdef REMOTE_VERSION
int server_init() {
    socklen_t server_len;
    struct sockaddr_in server_address;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    int reuse = 1, res;
    res = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    if(res < 0) { perror("Set Socket Option"); exit(1); }
    server_len = sizeof(server_address);
    res = bind(fd, (struct sockaddr *)&server_address, server_len);
    if(res < 0) { perror("Bind Error"); exit(1); }
    res = listen(fd, 500);
    if(res < 0) { perror("Listen Error"); exit(1); }
    return fd;
}
#endif

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

#ifdef LOCAL_VERSION
conn_t *
get_connection(FILE *f) {
    request_t *t = parse_args(f);
    if(strcmp(t->argv[0], P_CONNECT) == 0) {
        char *tmp = strlcat(CLIENT_FIFO, t->argv[1]);
        char *client_fifo_path_c = strlcat(tmp, "_c.fifo");
        char *client_fifo_path_d = strlcat(tmp, "_d.fifo");
        free(tmp);

        conn_t *conn = (conn_t*) malloc(sizeof(conn_t));
        conn->input = fopen(client_fifo_path_c, "r");
        conn->dfd = open(client_fifo_path_d, O_WRONLY);
        return conn;
    }
    return NULL;
}
#endif

#ifdef REMOTE_VERSION
conn_t *get_connection(int fd) {
    conn_t *conn = (conn_t*) malloc(sizeof(conn_t));
    struct sockaddr_in client_address;
    int client_fd;
    socklen_t client_len = sizeof(client_address);
    client_fd = accept(fd, (struct sockaddr *)&client_address, &client_len);
    puts("Good");
    if(client_fd == -1) { perror("Accept Error"); exit(1); }
    conn->input = fdopen(client_fd, "r");
    conn->dfd = client_fd;
    return conn;
}
#endif

void
release_conn(conn_t *c) {
    fclose(c->input);
    close(c->dfd);
}
