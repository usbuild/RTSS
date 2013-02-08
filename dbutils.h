#ifndef __DB_UTILS_H__
#define __DB_UTILS_H__

sqlite3 *rtss_db;
void db_init();
typedef int (*sqlcallback)(void *, int, char **, char **);
int exec_query(const char *sql, sqlcallback cbk);
#endif
