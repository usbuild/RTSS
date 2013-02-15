#include <service.h>
#include <malloc.h>
#include <protocol.h>
#include <utils.h>
#include <rpc_fifo_client.h>
#include <stdarg.h>
request_t *
new_request(char *protocol, int argc) {
    request_t *rqst;
    rqst = (request_t*) malloc(sizeof(request_t));
    rqst->argv = (char**) malloc(sizeof(char*) * 3);
    rqst->argc = argc;
    rqst->argv[0] = protocol;
    return rqst;
}
void
del_request(request_t *rqst) {
    free(rqst->argv);
    free(rqst);
}

int simple_query(conn_t *conn, char *protocol, int argc, ...) {
    va_list ap;
    int i;
    request_t *rqst = new_request(protocol, argc);
    va_start(ap, argc);
    for (i = 1; i < argc; ++i) {
        rqst->argv[i] = va_arg(ap, char *);
    }
    va_end(ap);
    send_data(conn, rqst);

    char symbol;
    char resp[10];
    read(conn->dfd, &symbol, 1);
    if(symbol == '+') {
        read(conn->dfd, resp, 2);
        return 0;
    } else {
        read(conn->dfd, resp, 4);
        return 1;
    }
}

int
login(char *username, char *password, conn_t *conn) {
    return simple_query(conn, P_LOGIN, 3,username, password);
}
int 
signup(char *username, char *password, conn_t *conn) {
    return simple_query(conn, P_SIGNUP, 3, username, password);
}

int 
buy(char *ticketid, conn_t *conn) {
    return simple_query(conn, P_BUY, 2, ticketid);
}
