#include <sqlite3.h>
#include <stdio.h>
#include "dbutils.h"
#include "config.h"
static
sqlite3* db_open(const char *dbname) {
    sqlite3 *db;
    int rc = sqlite3_open(dbname, &db);
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

void db_init() {
    rtss_db = db_open(DB_NAME);
}
