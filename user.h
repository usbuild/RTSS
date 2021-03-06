#ifndef __USER_H__
#define __USER_H__
typedef struct {
    char id[15];
    char password[40];
    char card[40];
    char phone[15];
    int  type;
} t_user;

t_user* find_user_by_id(const char* id);
int insert_user(t_user* data);
int update_user(const char *id, t_user *data);
int delete_user_by_id(const char *id);
#endif
