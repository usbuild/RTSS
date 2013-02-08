#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

void init() { // init the system
    
}


int callback(void *nouse, int argc, char **argv, char **col_name) {
    int i;
    for (i = 0; i < argc; ++i){
        printf("%s = %s\n", col_name[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
int main(int argc, const char *argv[])
{
    sqlite3 *db;
    const char *dbname = "test.db";
    char *err_message;
    int rc = sqlite3_open(dbname, &db);
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    const char *sql = "INSERT INTO test(id, name) values(2, 'world')";
    rc = sqlite3_exec(db, sql, callback, 0, &err_message);
    return 0;
}
