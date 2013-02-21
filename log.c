#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <log.h>
#include <config.h>
FILE *log_file = NULL;
void rtss_log(const char *format, ...) {
    if(log_file == NULL) log_file = fopen(LOG_FILE, "a");

    struct tm *p;
    time_t t;
    t = time(NULL);
    p = localtime(&t);

    char str[50];

    strftime(str, 50, "%Y-%m-%d %H:%M:%S", p);
    fprintf(log_file, "%s : ", str);
    va_list ap;
    va_start(ap, format);
    vfprintf(log_file, format, ap);
    va_end(ap);
    fprintf(log_file, "\n");
}
