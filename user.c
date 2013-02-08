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

t_user*
find_user_by_id(const char* id) {
    char sql[128];
    sprintf(sql, "SELECT * FROM user WHERE id = '%s'", id);
    exec_query(sql, callback);
}

int update_user(const char *id, t_user* data) {
    tmp_user = NULL; 
    find_user_by_id(id);
    char sql[128];

    if(tmp_user == NULL) {
        sprintf(sql,
            "INSERT INTO user(`id`, `password`, `card`, `phone`) values('%s', '%s', '%s', '%s')",
            data->id, data->password, data->card, data->phone);
    } else {
        sprintf(sql, 
            "UPDATE user SET id='%s', password='%s', card='%s', phone='%s' WHERE id='%s'",
            data->id, data->password, data->card, data->phone, id);
    }
    return exec_query(sql, callback);
}

int
delete_user_by_id(const char *id) {
    char sql[128];
    sprintf(sql, 
        "DELETE FROM user WHERE id='%s'",
        id);
    return exec_query(sql, callback);
}
