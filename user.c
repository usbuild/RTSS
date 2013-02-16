#include <user.h>
#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

t_user *tmp_user = NULL;

void clean_tmp_user(){
    if(tmp_user != NULL) {
        free(tmp_user);
        tmp_user = NULL;
    }
}

static int
callback(void *nouse, int argc, char **argv, char **col_name) {
    if(tmp_user != NULL) free(tmp_user);
    tmp_user = (t_user*) malloc(sizeof(t_user));
    strcpy(tmp_user->id, argv[0]);
    strcpy(tmp_user->password, argv[1]);
    strcpy(tmp_user->card, argv[2]);
    strcpy(tmp_user->phone, argv[3]);
    return 0;
}

t_user*
find_user_by_id(const char* id) {
    char sql[SQL_LEN];
    sprintf(sql, "SELECT * FROM user WHERE id = '%s'", id);
    clean_tmp_user();
    exec_query(sql, callback);
    return tmp_user;
}

int 
insert_user(t_user* data) {
    char sql[SQL_LEN];
    sprintf(sql,
        "INSERT INTO user(`id`, `password`, `card`, `phone`) values('%s', '%s', '%s', '%s')",
        data->id, data->password, data->card, data->phone);
    clean_tmp_user();
    return exec_query(sql, callback);
}

int
update_user(const char *id, t_user* data) {
    char sql[SQL_LEN];
    sprintf(sql, 
        "UPDATE user SET id='%s', password='%s', card='%s', phone='%s' WHERE id='%s'",
        data->id, data->password, data->card, data->phone, id);
    clean_tmp_user();
    return exec_query(sql, callback);
}

int
delete_user_by_id(const char *id) {
    char sql[SQL_LEN];
    sprintf(sql, 
        "DELETE FROM user WHERE id='%s'",
        id);
    clean_tmp_user();
    return exec_query(sql, callback);
}
