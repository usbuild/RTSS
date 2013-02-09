#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ticket.h>
#include <config.h>
#define DEFAULT_LIST_SIZE 10

t_ticket_list *tmp_tickets;
t_ticket *tmp_ticket;

static void
convert_to_t_ticket(char **argv, t_ticket *tkt) {
    strcpy(tkt->id, argv[0]);
    strcpy(tkt->start, argv[1]);
    strcpy(tkt->end, argv[2]);
    strcpy(tkt->stime, argv[3]);
    strcpy(tkt->etime, argv[4]);
    tkt->price = strtod(argv[5], NULL);
    tkt->distance = atoi(argv[6]);
    tkt->num = atoi(argv[7]);
}

static int
list_callback(void *nouse, int argc, char **argv, char **col_name) {
    if(tmp_tickets == NULL) {
        tmp_tickets = (t_ticket_list*) calloc( sizeof(t_ticket_list), 1);
    }
    if(tmp_tickets->num == 0) {
        tmp_tickets->total = DEFAULT_LIST_SIZE;
        tmp_tickets->data = (t_ticket*) calloc(sizeof(t_ticket), tmp_tickets->total);
    }
    if(tmp_tickets->num >= tmp_tickets->total) {
        tmp_tickets->total = 1.5 * tmp_tickets->num;
        tmp_tickets->data = (t_ticket*) realloc(tmp_tickets->data, tmp_tickets->total);
    }
    convert_to_t_ticket(argv, &tmp_tickets->data[tmp_tickets->num++]);
}

static int
callback(void *nouse, int argc, char **argv, char **col_name) {
    if(tmp_ticket != NULL) free(tmp_ticket);
    tmp_ticket = (t_ticket*) malloc(sizeof(t_ticket));
    convert_to_t_ticket(argv, tmp_ticket);
}

t_ticket_list *
search_tickets(const char *s, const char *e) {
    char sql[SQL_LEN];    
    sprintf(sql, 
        "SELECT * FROM ticket WHERE start like '%%%s%%' and '%%%s%%'",
        s, e);
    if(tmp_tickets != NULL) {
        free(tmp_tickets->data);
        free(tmp_tickets);
        tmp_tickets = NULL;
    }
    exec_query(sql, list_callback);
    return tmp_tickets;
}

t_ticket *
find_ticket_by_id(const char *id) {
    char sql[SQL_LEN];
    sprintf(sql,
        "SELECT * FROM ticket WHERE id='%s'",
        id);
    exec_query(sql, callback);
    return tmp_ticket;
}

int 
insert_ticket(t_ticket *tkt) {
    char sql[256];
    sprintf(sql, 
        "INSERT INTO ticket(`id`, `start`, `end`, `stime`, `etime`, `price`, `distance`, `num`)" \
        " values('%s', '%s', '%s', '%s', '%s', '%lf', '%d', '%d')",
        tkt->id,
        tkt->start,
        tkt->end,
        tkt->stime,
        tkt->etime,
        tkt->price,
        tkt->distance,
        tkt->num
        );
    puts(sql);
    return exec_query(sql, callback);
}

int
update_ticket(const char *id, t_ticket *tkt) {
    char sql[SQL_LEN];
    sprintf(sql,
        "UPDATE ticket SET id = '%s', start='%s', end='%s', stime='%s', etime='%s', price='%lf', distance='%d', num='%d' WHERE id='%s'",
        tkt->id,
        tkt->start,
        tkt->end,
        tkt->stime,
        tkt->etime,
        tkt->price,
        tkt->distance,
        tkt->num,
        id
        );
    return exec_query(sql, callback);
}
int
delete_ticket_by_id(const char *id) {
    char sql[SQL_LEN];
    sprintf(sql, 
        "DELETE FROM ticket WHERE id='id'",
        id);
    return exec_query(sql, callback);
}