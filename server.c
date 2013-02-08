#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "dbutils.h"
#include "user.h"

static int
callback(void *nouse, int argc, char **argv, char **col_name) {
    int i;
    for (i = 0; i < argc; ++i){
        printf("%s = %s\n", col_name[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void init() { // init the system
    db_init();
}


int main(int argc, const char *argv[])
{
    init();
    /*
    t_user u;
    strcpy(u.id, "abc");
    strcpy(u.password, "abcde");
    strcpy(u.card, "342622");
    strcpy(u.phone, "1234567");
    update_user("abc", &u);
    */
    delete_user_by_id("abc");
    return 0;
}
