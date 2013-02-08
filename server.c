#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
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
    find_user_by_id("usbuild");
    return 0;
}
