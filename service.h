#ifndef __SERVICE_H__
#define __SERVICE_H__
#include <rpc_fifo_client.h>
typedef struct {
    char id[15];
    char password[40];
    char card[40];
    char phone[15];
} t_user;

typedef struct {
    char    id[10];
    char    start[15];
    char    end[15];
    char  stime[20];
    char  etime[20];
    double  price;
    int     distance;
    int     num;
} t_ticket;

typedef struct {
    t_ticket *data;
    int num;
} t_ticket_list;

int login(char *username, char *password, conn_t *conn);

int signup(char *username, char *password, conn_t *conn);

int buy(char *ticketid, conn_t *conn);

t_user *user_info(conn_t *conn);

t_ticket_list * query_ticket(char *site1, char *site2, conn_t *conn);

int update_user(char *passwd, char *card, char *phone, conn_t *conn);
t_ticket_list  *query_buy_ticket(conn_t *conn);
#endif
