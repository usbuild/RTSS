#ifndef __SERVICE_H__
#define __SERVICE_H__
#include <rpc_client.h>
typedef struct {
    char id[15];
    char password[40];
    char card[40];
    char phone[15];
    int  type;
} t_user;

typedef struct {
    char    id[10];
    int     start_id;
    int     end_id;
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

typedef struct {
    int id;
    char name[20];
} t_station;

typedef struct {
    t_station *data;
    int num;
} t_station_list;

int login(char *username, char *password, conn_t *conn);

int signup(char *username, char *password, conn_t *conn);

int buy(char *ticketid, conn_t *conn);

t_user *user_info(conn_t *conn);

t_ticket_list * query_ticket(char *site1, char *site2, conn_t *conn);

int update_user(char *passwd, char *card, char *phone, conn_t *conn);
t_ticket_list  *query_buy_ticket(conn_t *conn);
t_ticket *load_ticket(char *, conn_t *);


int 
update_tkt(char *oldid,
    char *id,
    char *start,
    char *end,
    char *stime,
    char *etime,
    char *price,
    char *distance,
    char *num,
    conn_t *conn
    );

int del_station(char *id, conn_t *conn);
int add_station(char *name, conn_t *conn);
t_station_list * all_station(conn_t *conn);
void client_exit(conn_t *conn);
#endif
