#include <service.h>
#include <string.h>
#include <malloc.h>
#include <protocol.h>
#include <utils.h>
#include <rpc_fifo_client.h>
#include <stdarg.h>
#define LINE_BUF 1024
request_t * new_request(char *protocol, int argc) {
    request_t *rqst;
    rqst = (request_t*) malloc(sizeof(request_t));
    rqst->argv = (char**) malloc(sizeof(char*) * argc);
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

    del_request(rqst);

    char line[LINE_BUF] = {0};
    fgets(line, LINE_BUF, conn->input);

    if(line[0] == '+') {
        return 0;
    } else {
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

t_ticket_list  *
query_ticket(char *site1, char *site2, conn_t *conn) {
    request_t *rqst = new_request(P_QUERY, 3);
    rqst->argv[1] = site1;
    rqst->argv[2] = site2;
    send_data(conn, rqst);
    del_request(rqst);

    char line[LINE_BUF] = {0};
    fgets(line, LINE_BUF, conn->input);
    t_ticket_list *list = (t_ticket_list*) malloc(sizeof(t_ticket_list));
    line[strlen(line) - 2] = 0;
    list->num = atoi(line + 1);
    list->data = (t_ticket *) calloc(list->num, sizeof(t_ticket));
    int i;
    for (i = 0; i < list->num; ++i) {
        fgets(line, LINE_BUF, conn->input);
        fread(&list->data[i], sizeof(t_ticket), 1, conn->input);
    }
    return list;
}

t_ticket_list *
query_buy_ticket(conn_t *conn) {
    request_t *rqst = new_request(P_QUERY_BUY, 1);
    send_data(conn, rqst);
    del_request(rqst);

    char line[LINE_BUF] = {0};
    fgets(line, LINE_BUF, conn->input);
    t_ticket_list *list = (t_ticket_list*) malloc(sizeof(t_ticket_list));
    line[strlen(line) - 2] = 0;
    list->num = atoi(line + 1);
    list->data = (t_ticket *) calloc(list->num, sizeof(t_ticket));
    int i;
    for (i = 0; i < list->num; ++i) {
        fgets(line, LINE_BUF, conn->input);
        fread(&list->data[i], sizeof(t_ticket), 1, conn->input);
    }
    return list;
}

t_ticket *
load_ticket(char *id, conn_t *conn) {
    request_t *rqst = new_request(P_TICKET, 2);
    rqst->argv[1] = id;
    send_data(conn, rqst);
    del_request(rqst);

    char line[LINE_BUF] = {0};
    fgets(line, LINE_BUF, conn->input);
    t_ticket *tkt = NULL;
    if(line[0] != '-') {
        tkt = (t_ticket*) malloc(sizeof(t_ticket));
        fread(tkt, sizeof(t_ticket), 1, conn->input);
    }
    return tkt;
}

t_user *
user_info(conn_t *conn) {
    request_t *rqst = new_request(P_USER_INFO, 1);
    send_data(conn, rqst);
    del_request(rqst);
    char line[LINE_BUF] = {0};
    fgets(line, LINE_BUF, conn->input);
    if(line[0] == '-') return NULL;
    t_user *user = (t_user*) malloc(sizeof(t_user));
    fread(user, sizeof(t_user), 1, conn->input);
    return user;
}

int
update_user(char *passwd, char *card, char *phone, conn_t *conn) {
    return simple_query(conn, P_USER_UPD, 4, passwd, card, phone);
}

int 
refund(char *id, conn_t *conn) {
    return simple_query(conn, P_REFUND, 2, id);
}

int 
update_tkt(char *oldid,/*{{{*/
    char *id,
    char *start,
    char *end,
    char *stime,
    char *etime,
    char *price,
    char *distance,
    char *num,
    conn_t *conn
    ) {
    return simple_query(conn, P_TKT_UPDATE, 10,
        oldid,
        id,
        start,
        end,
        stime,
        etime,
        price,
        distance,
        num
        );
}/*}}}*/
int del_station(char *id, conn_t *conn) {
    return simple_query(conn, P_STN_DEL, 2, id);
}
int add_station(char *name, conn_t *conn) {
    return simple_query(conn, P_STN_ADD, 2, name);
}
t_station_list * all_station(conn_t *conn) {
    request_t *rqst = new_request(P_STN_ALL, 1);
    send_data(conn, rqst);
    del_request(rqst);

    char line[LINE_BUF] = {0};
    fgets(line, LINE_BUF, conn->input);
    t_station_list *list = (t_station_list*) malloc(sizeof(t_station_list));

    line[strlen(line) - 2] = 0;
    list->num = atoi(line + 1);
    list->data = (t_station *) calloc(list->num, sizeof(t_station));
    int i;
    for (i = 0; i < list->num; ++i) {
        fgets(line, LINE_BUF, conn->input);
        fread(&list->data[i], sizeof(t_station), 1, conn->input);
    }
    return list;
}
