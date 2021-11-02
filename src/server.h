#ifndef SERVER_H
#define SERVER_H

// Include headers here
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <regex.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

// Define universal symbols here
#define TRUE 1
#define FALSE 0

// Define length limits here
#define MAX_BUF 2048
#define MAX_LOG_BUF 512

#define MAX_CLIENT 1024

#define MAX_URI_LEN 1024
#define MAX_REQ_LEN (MAX_URI_LEN + 1024)
#define MAX_RESP_CODE_LEN 56
#define MAX_RESP_LINE_LEN (MAX_RESP_CODE_LEN + 16)
#define MAX_DATE_LEN 30

// Define error methods here
#define INFO 0
#define WARN 1
#define ERR 2

// Define Methods here
#define GET 0
#define HEAD 1
#define POST 2

/*----------------------------------------------------- server header ---------------------------------------------*/

int init_server(char *host, char *server, int family, int protocol, int socktype);
void cleanup(void);

/*----------------------------------------------------- http header ---------------------------------------------*/

/* Custom functions return 0 in failure else non zero */

/* A client type for the linked list of clients.
 * Members :
 * 	1) Client file descriptor
 * 	2) Length of struct containing address
 * 	3) Sturct address
 * 	4) Pointer to buffer for the client
 * 	5) Pointer to the next clien tin the list
 */

typedef struct client{
	int cfd;
	ssize_t readData;
	socklen_t socklen;
	struct client *client_next;
	struct sockaddr addr;
	char host[NI_MAXHOST];
	char readBuf[MAX_BUF];

} CLIENT;

/* Repsonse Structure */
typedef struct response{
	char response_line[MAX_RESP_LINE_LEN];
	char **headers;
} RESPONSE;

CLIENT* get_client(int fd);

void wait_on_client(int servfd);
int add_client(int fd, struct sockaddr *addr, socklen_t *socklen);
void drop_client(int fd);

void serve_resource(CLIENT* cl);

/*----------------------------------------------------- log header ---------------------------------------------*/

void server_log(int mode, char *fmt,...);
void client_log(char *host,int mode, char *fmt,...);

ssize_t ws_read(int fd, void *buf, size_t count);
ssize_t ws_write(int fd, void *buf, size_t count);

/*----------------------------------------------------- error_code header ---------------------------------------------*/

typedef struct resp_code {
	int code;
	char str[MAX_RESP_CODE_LEN];
} RESP_CODE;

void send_code(int err, char *host, int fd, int close, size_t body);

/*----------------------------------------------------- parse header ---------------------------------------------*/

char *parse_url(char *str);
int parse_request_line(char *host, char *str, int fd);
int parse_header(char *host, char *str, int fd);
int parse_request(char *host, char *str, int fd);

/*----------------------------------------------------- end header ---------------------------------------------*/
#endif
