#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ltoa(long data) {
    static char s[20];
    sprintf(s, "%ld", data);
    return s;
}

char *strlcat(char *str1,char *str2) {
    char *s = (char*) calloc(strlen(str1) + strlen(str2) + 1, sizeof(char));
    strcpy(s, str1);
    strcat(s, str2);
    return s;
}
