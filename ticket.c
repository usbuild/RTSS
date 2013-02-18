#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ticket.h>
#include <config.h>
#define DEFAULT_LIST_SIZE 10

t_ticket_list *tmp_tickets = NULL;
t_ticket *tmp_ticket;

static void 
clean_tmp_ticket() {
    if(tmp_ticket != NULL) {
        free(tmp_ticket);
        tmp_ticket = NULL;
    }
}

static void 
clean_tmp_tickets() {
    if(tmp_tickets != NULL) {
        if (tmp_tickets->data != NULL) {
            free(tmp_tickets->data);
        }
    } else {
        tmp_tickets = (t_ticket_list*) malloc(sizeof(t_ticket_list));
    }
    tmp_tickets->data = NULL;
    tmp_tickets->num = 0;
    tmp_tickets->total = 0;
}

static void
convert_to_t_ticket(char **argv, t_ticket *tkt) {
    strcpy(tkt->id, argv[0]);
    tkt->start_id = atoi(argv[1]);
    tkt->end_id = atoi(argv[2]);
    strcpy(tkt->stime, argv[3]);
    strcpy(tkt->etime, argv[4]);
    tkt->price = strtod(argv[5], NULL);
    tkt->distance = atoi(argv[6]);
    tkt->num = atoi(argv[7]);
    strcpy(tkt->start, argv[8]);
    strcpy(tkt->end, argv[9]);
}

static int
list_callback(void *nouse, int argc, char **argv, char **col_name) {
    if(tmp_tickets->num == 0) {
        tmp_tickets->total = DEFAULT_LIST_SIZE;
        tmp_tickets->data = (t_ticket*) calloc(sizeof(t_ticket), tmp_tickets->total);
    }
    if(tmp_tickets->num >= tmp_tickets->total) {
        tmp_tickets->total *= 1.5;
        tmp_tickets->data = (t_ticket*) realloc(tmp_tickets->data, tmp_tickets->total);
    }
    convert_to_t_ticket(argv, &tmp_tickets->data[tmp_tickets->num++]);
    return 0;
}

static int
callback(void *nouse, int argc, char **argv, char **col_name) {
    tmp_ticket = (t_ticket*) malloc(sizeof(t_ticket));
    convert_to_t_ticket(argv, tmp_ticket);
    return 0;
}

t_ticket_list *
search_tickets(const char *s, const char *e) {
    char sql[SQL_LEN];    
    sprintf(sql, 
        "SELECT t.*, s1.name, s2.name  FROM ticket t " \
        "LEFT JOIN station s1 ON s1.id = t.start_id LEFT JOIN station s2 ON s2.id = t.end_id" \
        " WHERE s1.name LIKE '%%%s%%' and s2.name LIKE '%%%s%%' ",
        s, e);
    clean_tmp_tickets();
    exec_query(sql, list_callback);
    return tmp_tickets;
}

//-----------use user_ticket code table
t_ticket_list *
find_tickets_by_user_id(const char *user_id) {
    char sql[SQL_LEN];
    sprintf(sql,
        "SELECT t.*, s1.name, s2.name FROM user_ticket ut "\
        "LEFT JOIN ticket t ON ut.ticket_id = t.id " \
        "LEFT JOIN station s1 ON s1.id = t.start_id LEFT JOIN station s2 ON s2.id = t.end_id " \
        "WHERE ut.user_id = '%s' ",
        user_id);

    clean_tmp_tickets();
    exec_query(sql, list_callback);
    return tmp_tickets;
}

int
has_user_bought(const char *id, const char *ticket_id) {
    char sql[SQL_LEN];
    sprintf(sql, 
        "SELECT t.*,s1.name, s2.name FROM user_ticket ut, ticket t "\
        "LEFT JOIN station s1 ON s1.id = t.start_id LEFT JOIN station s2 ON s2.id = t.end_id " \
        "where ut.ticket_id = '%s' and ut.user_id = '%s' and ut.ticket_id = t.id",
        ticket_id, id);
    clean_tmp_ticket();
    exec_query(sql, callback);
    if(tmp_ticket != NULL) return 1;
    return 0;
}

int 
add_user_ticket(const char *user_id, const char *ticket_id) {
    char sql[SQL_LEN];
    sprintf(sql, 
        "INSERT INTO user_ticket(`id`, `user_id`, `ticket_id`) VALUES(NULL, '%s', '%s')",
        user_id, ticket_id
        );
    clean_tmp_ticket();
    return exec_query(sql, callback);
}
int 
del_user_ticket(const char *user_id, const char *ticket_id) {
    char sql[SQL_LEN];
    sprintf(sql,
        "DELETE FROM user_ticket WHERE user_id='%s' AND ticket_id = '%s'",
        user_id, ticket_id
        );
    clean_tmp_ticket();
    return exec_query(sql, callback);
}


t_ticket *
find_ticket_by_id(const char *id) {
    char sql[SQL_LEN];
    sprintf(sql,
        "SELECT t.*, s1.name, s2.name FROM ticket t "\
        "LEFT JOIN station s1 ON s1.id = t.start_id LEFT JOIN station s2 ON s2.id = t.end_id " \
        "WHERE t.id='%s' ",
        id);
    clean_tmp_ticket();
    exec_query(sql, callback);
    return tmp_ticket;
}

int 
insert_ticket(t_ticket *tkt) {
    char sql[256];
    sprintf(sql, 
        "INSERT INTO ticket(`id`, `start_id`, `end_id`, `stime`, `etime`, `price`, `distance`, `num`)" \
        " VALUES('%s', '%d', '%d', '%s', '%s', '%lf', '%d', '%d')",
        tkt->id,
        tkt->start_id,
        tkt->end_id,
        tkt->stime,
        tkt->etime,
        tkt->price,
        tkt->distance,
        tkt->num
        );
    clean_tmp_ticket();
    return exec_query(sql, callback);
}

int
update_ticket(const char *id, t_ticket *tkt) {
    char sql[SQL_LEN];
    sprintf(sql,
        "UPDATE ticket SET id = '%s', start_id='%d', end_id='%d', stime='%s', etime='%s', price='%lf', distance='%d', num='%d' WHERE id='%s'",
        tkt->id,
        tkt->start_id,
        tkt->end_id,
        tkt->stime,
        tkt->etime,
        tkt->price,
        tkt->distance,
        tkt->num,
        id
        );
    clean_tmp_ticket();
    return exec_query(sql, callback);
}
int
delete_ticket_by_id(const char *id) {
    char sql[SQL_LEN];
    sprintf(sql, 
        "DELETE FROM ticket WHERE id='%s'",
        id);
    clean_tmp_ticket();
    return exec_query(sql, callback);
}

