#ifndef __CONFIG_H__
#define __CONFIG_H__

#define DB_NAME "rtss.db"
#define SQL_LEN 256
#define ARG_LEN 128
#define LOG_FILE "/tmp/rtss.log"

#define PORT 4327
#define IP "127.0.0.1"

#define CRLF "\r\n"
#define CLLEN 2

//#define LOCAL_VERSION
#define REMOTE_VERSION

#ifdef LOCAL_VERSION
#define SERVER_FIFO "/tmp/rtss_server.fifo"
#define CLIENT_FIFO "/tmp/rtss_client_"
#endif

#endif
