#ifndef __RPC_FIFO_CLIENT_H__
#define __RPC_FIFO_CLIENT_H__
#include <config.h>
#include <utils.h>

#ifdef LOCAL_VERSION
typedef struct {
    int cfd;        //control fd
    int dfd;        //data fd
    char *cpath;
    char *dpath;
} conn_t;
#endif

#ifdef LOCAL_VERSION

#endif

conn_t *init_client();
void release_connection(conn_t *conn);
void send_data(conn_t *conn, request_t *request);
#endif
