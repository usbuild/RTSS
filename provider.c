#include <provider.h>
#include <protocol.h>
#include <rpc_fifo_server.h>
#include <ticket.h>
#include <user.h>
int login(char *username, char *password) {
    t_user *user = find_user_by_id(username);
    if(user == NULL) return 1;
    return strcmp(username, user->id);
}
void handle_client(conn_t *conn) {
    for(; ;) {
        request_t *rqst = parse_args(conn->input);
        if(rqst != NULL) {
            if(strcmp(rqst->argv[0], P_LOGIN) == 0) {
                char *ok = "+OK";
                char *deny = "-Deny";
                if(login(rqst->argv[1], rqst->argv[2]) == 0) {
                    write(conn->dfd, ok, strlen(ok));
                } else {
                    write(conn->dfd, deny, strlen(deny));
                }
            }
        }
    }
}
