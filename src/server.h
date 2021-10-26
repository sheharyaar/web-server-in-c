#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#include "log.h"

#define TRUE 1
#define FALSE 0

#define MAX_CLIENT 1024

int init_server(char *host, char *server, int family, int protocol, int socktype);
void cleanup(void);

#endif
