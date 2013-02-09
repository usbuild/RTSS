#ifndef __TICKET_H__
#define __TICKET_H__
#include <sys/types.h>
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
    t_ticket* data;
    int total;
    int num;
} t_ticket_list;

t_ticket_list* search_tickets(const char *s, const char *e);
t_ticket *find_ticket_by_id(const char *id);
int insert_ticket(t_ticket *tkt);
int update_ticket(const char *id, t_ticket *tkt);
int delete_ticket_by_id(const char *id);

#endif
