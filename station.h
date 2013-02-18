#ifndef __STATION_H__
#define __STATION_H__
typedef struct {
    int id;
    char name[20];
} t_station;

typedef struct {
    t_station *data;
    int num;
    int total;
} t_station_list;

t_station_list * list_station();
int remove_station(int id);
int insert_station(char *name);
#endif
