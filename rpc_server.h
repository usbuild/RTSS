#ifndef __RPC_FIFO_SERVER_H__
#define __RPC_FIFO_SERVER_H__
#include <config.h>
#include <stdio.h>
#include <utils.h>
#include <user.h>

typedef struct {
    FILE *input;
    int dfd;        //data fd
    t_user *user;
} conn_t;

#ifdef LOCAL_VERSION
FILE *server_init(const char *path);
conn_t *get_connection(FILE *f);
#endif

#ifdef REMOTE_VERSION
int server_init();
conn_t *get_connection(int fd);
#endif


void send_data(conn_t *conn, void *data, long length);
void receive_data(conn_t *conn);
void release_conn(conn_t *c);
request_t * parse_args(FILE *file);
#endif
