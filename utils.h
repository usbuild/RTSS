#ifndef __UTILS_H__
#define __UTILS_H__

typedef struct {
    int argc;
    char **argv;
} request_t;

char *ltoa(long data);
char *strlcat(char *str1, char *str2);
void free_request_t(request_t *request);

#endif
