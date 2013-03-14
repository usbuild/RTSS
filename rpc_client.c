#include <rpc_client.h>
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <protocol.h>
#include <string.h>
#include <utils.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

#ifdef REMOTE_VERSION
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#define BUF_LEN 1024

extern int errno;

char *build_request_str(request_t *rqst) {
    char *str = (char*) malloc(sizeof(char) * 1024);

    char *tmp = ltoa(rqst->argc);
    strcpy(str, "=");
    strcat(str, tmp);
    strcat(str, CRLF);
    free(tmp);

    int i;
    for (i = 0; i < rqst->argc; ++i) {
        strcat(str, "~");
        strcat(str, rqst->argv[i]);
        strcat(str, CRLF);
    }
    puts(str);
    return str;
}
#ifdef LOCAL_VERSION
conn_t *connect_server() {
    pid_t pid = getpid();
    request_t rqst;
    rqst.argc = 2;
    rqst.argv = (char**) malloc(sizeof(char*) * rqst.argc);
    rqst.argv[0] = P_CONNECT;
    char *tmp = ltoa(pid);
    rqst.argv[1] = tmp;
    char *str = build_request_str(&rqst);
    free(rqst.argv);
    conn_t *ct = (conn_t*) malloc(sizeof(conn_t));
    char *t = strlcat(CLIENT_FIFO, tmp);
    char *path_c = strlcat(t, "_c.fifo");
    char *path_d = strlcat(t, "_d.fifo");
    ct->cpath = path_c;
    ct->dpath = path_d;
    free(tmp);
    free(t);

    int rc = mkfifo(path_c, S_IRUSR | S_IWUSR);
    if(rc && errno != EEXIST) {
        perror("Create Data FIFO");
        return NULL;
    }

    rc = mkfifo(path_d, S_IRUSR | S_IWUSR);
    if(rc && errno != EEXIST) {
        perror("Create Command FIFO ");
        return NULL;
    }

    errno = 0;
    rc = open(SERVER_FIFO, O_WRONLY);
    if(errno) {
        perror("Connect Server");
        exit(1);
    }

    write(rc, str, strlen(str));
    free(str);

    ct->cfd = open(path_c, O_WRONLY);
    ct->input = fopen(ct->dpath, "r");
    return ct;
}
#endif

#ifdef REMOTE_VERSION
conn_t *connect_server() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int res;
    struct hostent *server = gethostbyname("localhost");
    if(server == NULL) {perror("gethost Error");exit(1);}
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    res = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(res < 0) {perror("Connect Error");exit(1);}
    conn_t *conn = (conn_t *) malloc(sizeof(conn_t));
    conn->cfd = fd;
    conn->input = fdopen(fd, "r");
    return conn;
}
#endif


void
release_connection(conn_t *conn) {
    close(conn->cfd);
    fclose(conn->input);
#ifdef LOCAL_VERSION
    unlink(conn->cpath);
    unlink(conn->dpath);
#endif
}
void send_data(conn_t *conn, request_t *request) {
    char *str = build_request_str(request);
    write(conn->cfd, str, strlen(str));
    free(str);
}
