#ifndef __RPC_FIFO_SERVER_H__
#define __RPC_FIFO_SERVER_H__
#include <config.h>

#ifdef LOCAL_VERSION
typedef struct {
    int fd;
} conn_t;
#endif

typedef struct {
    int argc;
    char **argv;
} request_t;

#ifdef LOCAL_VERSION
FILE *server_init(const char *path);
#endif


conn_t *get_connection(FILE *f);
void send_data(conn_t *conn, void *data, long length);
void receive_data(conn_t *conn);
#endif
