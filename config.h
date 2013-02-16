#ifndef __CONFIG_H__
#define __CONFIG_H__

#define DB_NAME "rtss.db"
#define SQL_LEN 256
#define ARG_LEN 128

#define CRLF "\r\n"

#define LOCAL_VERSION

#ifdef LOCAL_VERSION
#define SERVER_FIFO "/tmp/rtss_server.fifo"
#define CLIENT_FIFO "/tmp/rtss_client_"
#endif

#endif
