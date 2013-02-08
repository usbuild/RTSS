#include <sqlite3.h>
#include <stdio.h>
#include "dbutils.h"
#include "config.h"

static sqlite3* 
db_open(const char *dbname) {
    sqlite3 *db;
    int rc = sqlite3_open(dbname, &db);
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

int
exec_query(const char *sql, sqlcallback cbk) {
    char *err_msg;
    int rc = sqlite3_exec(rtss_db, sql, cbk, 0, &err_msg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }
    return 0;
}

void db_init() {
    rtss_db = db_open(DB_NAME);
}
