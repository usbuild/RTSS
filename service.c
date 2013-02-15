#include <service.h>
#include <malloc.h>
#include <protocol.h>
#include <utils.h>
#include <rpc_fifo_client.h>
int login(char *username, char *password, conn_t *conn) {
    request_t request;
    request.argc = 3;
    request.argv = (char**)malloc(sizeof(char*) * 3);
    request.argv[0] = P_LOGIN;
    request.argv[1] = "usbuild";
    request.argv[2] = "12345";
    send_data(conn, &request);
    
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
