#include <utils.h>
#include <stdio.h>

char *ltoa(long data) {
    static char s[20];
    sprintf(s, "%ld", data);
    return s;
}
