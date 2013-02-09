#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <dbutils.h>
#include <user.h>
#include <ticket.h>

void init() { // init the system
    db_init();
}


int main(int argc, const char *argv[])
{
    init();
    t_ticket tkt;
    strcpy(tkt.id, "K8955");
    strcpy(tkt.start, "Nanjing");
    strcpy(tkt.end, "Beijing");
    strcpy(tkt.stime, "2013-02-09 12:00");
    strcpy(tkt.etime, "2013-02-09 20:00");
    tkt.price = 56.0;
    tkt.distance = 100;
    tkt.num = 500;
    insert_ticket(&tkt);
    return 0;
}
