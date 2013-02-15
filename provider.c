#include <provider.h>
#include <string.h>
#include <protocol.h>
#include <rpc_fifo_server.h>
#include <ticket.h>
#include <user.h>
#include <stdlib.h>
#define IS_PROTOCOL(A,B) (strcmp(A->argv[0], B) == 0)


int login(char *username, char *password) {
    t_user *user = find_user_by_id(username);
    if(user == NULL) return 1;
    return strcmp(username, user->id);
}

int signup(char *username, char *password) {
    t_user user;
    strcpy(user.id, username);
    strcpy(user.password, password);
    strcpy(user.card, "");
    strcpy(user.phone, "");
    return insert_user(&user);
}

int buy(char *username, char *ticketid) {
    return add_user_ticket(username, ticketid);
}

void simple_response(int status, conn_t *conn) {
    char *ok = "+OK";
    char *deny = "-Deny";
    if(status == 0) {
        write(conn->dfd, ok, strlen(ok));
    } else {
        write(conn->dfd, deny, strlen(deny));
    }
}
void handle_client(conn_t *conn) {
    conn->user = NULL;
    for(; ;) {
        request_t *rqst = parse_args(conn->input);
        if(rqst != NULL) {
            if(IS_PROTOCOL(rqst, P_LOGIN)) {
                if(login(rqst->argv[1], rqst->argv[2]) == 0) {
                    conn->user = (t_user*) malloc(sizeof(t_user));
                    memcpy(conn->user, find_user_by_id(rqst->argv[1]), sizeof(conn->user));
                    simple_response(0, conn);
                } else {
                    simple_response(1, conn);
                }
            } 
            else  if(conn->user != NULL) {
                if(IS_PROTOCOL(rqst, P_SIGNUP)) {
                    if(signup(rqst->argv[1], rqst->argv[2]) == 0 ) {
                        simple_response(0, conn);
                    } else {
                        simple_response(1, conn);
                    }
                } else if(IS_PROTOCOL(rqst, P_BUY)) {
                    if(buy(conn->user->id, rqst->argv[1]) == 0)  {
                        simple_response(0, conn);
                    } else {
                        simple_response(1, conn);
                    }
                }

            } else {
                simple_response(1, conn);
            }
        }
    }
}
