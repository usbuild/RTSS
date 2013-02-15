#ifndef __SERVICE_H__
#define __SERVICE_H__
#include <rpc_fifo_client.h>
int login(char *username, char *password, conn_t *conn);

int signup(char *username, char *password, conn_t *conn);

int buy(char *ticketid, conn_t *conn);
#endif
