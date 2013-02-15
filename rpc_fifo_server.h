#ifndef __RPC_FIFO_SERVER_H__
#define __RPC_FIFO_SERVER_H__
#include <config.h>
#include <stdio.h>
#include <utils.h>

#ifdef LOCAL_VERSION
typedef struct {
    FILE *input;
    int cfd;        //control fd
    int dfd;        //data fd
} conn_t;
#endif

#ifdef LOCAL_VERSION
FILE *server_init(const char *path);
#endif


conn_t *get_connection(FILE *f);
void send_data(conn_t *conn, void *data, long length);
void receive_data(conn_t *conn);
void release_conn(conn_t *c);
request_t * parse_args(FILE *file);
#endif
