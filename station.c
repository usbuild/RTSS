#include <station.h>
#include <stdlib.h>
#include <dbutils.h>
#include <string.h>
#include <stdio.h>
#include <config.h>

#define DEFAULT_LIST_SIZE 10

t_station_list *tmp_stations = NULL;
t_station *tmp_station = NULL;

static void
clean_tmp_station() {
    if(tmp_station == NULL) {
        free(tmp_station);
        tmp_station = NULL;
    }
}

static void 
clean_tmp_stations() {
    if(tmp_stations != NULL) {
        if(tmp_stations->data != NULL) free(tmp_stations->data);
    } else {
        tmp_stations = (t_station_list*) malloc(sizeof(t_station_list));
    }
    tmp_stations->data = NULL;
    tmp_stations->total = tmp_stations->num = 0;
}


static void 
convert_to_t_station(char **argv, t_station *st) {
    st->id = atoi(argv[0]);
    strcpy(st->name, argv[1]);
}

static int 
list_callback(void *nouse, int argc, char **argv, char **col_name) {
    if(tmp_stations->num == 0) {
        tmp_stations->total = DEFAULT_LIST_SIZE;
        tmp_stations->data = (t_station*) calloc(sizeof(t_station), tmp_stations->total);
    }
    if(tmp_stations->num >= tmp_stations->total) {
        tmp_stations->total *= 1.5;
        tmp_stations->data = (t_station*) realloc(tmp_stations->data, tmp_stations->total);
    }
    convert_to_t_station(argv, &tmp_stations->data[tmp_stations->num++]);
    return 0;
}

static int
callback(void *nouse, int argc, char **argv, char **col_name) {
    tmp_station = (t_station*) malloc(sizeof(t_station));
    convert_to_t_station(argv, tmp_station);
    return 0;
}

t_station_list * list_station() {
    char sql[SQL_LEN];
    sprintf(sql, "SELECT * FROM station");
    clean_tmp_stations();
    exec_query(sql, list_callback);
    return tmp_stations;
}

int remove_station(int id) {
    char sql[SQL_LEN];
    sprintf(sql, "DELETE FROM station WHERE id = %d", id);
    clean_tmp_station();
    return exec_query(sql, callback);
}

int insert_station(char *name) {
    char sql[SQL_LEN];
    sprintf(sql, "INSERT INTO station VALUES(NULL, '%s')", name);
    clean_tmp_station();
    return exec_query(sql, callback);
}
