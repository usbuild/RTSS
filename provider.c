#include <provider.h>
#include <string.h>
#include <protocol.h>
#include <rpc_fifo_server.h>
#include <ticket.h>
#include <user.h>
#include <stdlib.h>
#include <dbutils.h>
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
    if(has_user_bought(username, ticketid)) return 1;
    t_ticket *tkt = find_ticket_by_id(ticketid);
    if(tkt == NULL) return 1;
    if(tkt->num == 0) return 1;
    --tkt->num;

    tx_begin();
    int rc = update_ticket(tkt->id, tkt);
    if(rc) {
        tx_rollback();
        return 1;
    }
    rc = add_user_ticket(username, ticketid);
    if(rc) {
        tx_rollback();
        return 1;
    }
    tx_commit();
    return 0;
}

t_ticket_list* 
query(const char *site1, const char *site2) {
    return search_tickets(site1, site2);
}


void simple_response(int status, conn_t *conn) {
    char *ok = "+OK"CRLF;
    char *deny = "-Deny"CRLF;
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
                } else if(IS_PROTOCOL(rqst, P_QUERY)) {
                    t_ticket_list *list = query(rqst->argv[1], rqst->argv[2]);
                    write(conn->dfd, "*", 1);

                    int crlen = strlen(CRLF);
                    char *tmp = ltoa(list->num);

                    int tmplen = strlen(tmp);
                    write(conn->dfd, tmp, tmplen);
                    write(conn->dfd, CRLF, crlen);

                    free(tmp);

                    tmp = ltoa(sizeof(t_ticket));
                    tmplen = strlen(tmp);

                    int i;
                    for (i = 0; i < list->num; ++i) {
                        write(conn->dfd, "?", 1); write(conn->dfd, tmp, tmplen); write(conn->dfd, CRLF, crlen);
                        write(conn->dfd, &list->data[i], sizeof(t_ticket));
                    }
                    free(tmp);
                }

            } else {
                simple_response(1, conn);
            }
        }
    }
}