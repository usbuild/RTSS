#include "user.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
t_user *tmp_user = NULL;
static int callback(void *nouse, int argc, char **argv, char **col_name) {
    tmp_user = (t_user*) malloc(sizeof(t_user));
    strcpy(tmp_user->id, argv[0]);
    strcpy(tmp_user->password, argv[1]);
    strcpy(tmp_user->card, argv[2]);
    strcpy(tmp_user->phone, argv[3]);
    return 0;
}
t_user* find_user_by_id(const char* id) {
    char sql[128];
    sprintf(sql, "SELECT * FROM user WHERE id = '%s'", id);
    exec_query(sql, callback);
    puts(tmp_user->password);
}
