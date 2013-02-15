#ifndef __RPC_FIFO_CLIENT_H__
#define __RPC_FIFO_CLIENT_H__
#include <config.h>
#include <utils.h>

#ifdef LOCAL_VERSION
typedef struct {
    int fd;
} conn_t;
#endif

#ifdef LOCAL_VERSION

#endif

conn_t *init_client();
char *build_request_str(request_t *rqst);

#endif
