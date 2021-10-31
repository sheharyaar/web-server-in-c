#ifndef SERVER_H
#define SERVER_H

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

/*----------------------------------------------------- server header ---------------------------------------------*/

#define TRUE 1
#define FALSE 0

#define MAX_CLIENT 1024
#define MAX_BUF 2048
#define MAX_URI_LEN 1024
#define MAX_REQ_LEN 2048

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

#define RESP_LINE_LEN 56

typedef struct client{
	int cfd;
	ssize_t readData;
	socklen_t socklen;
	struct client *client_next;
	struct sockaddr addr;
	char host[NI_MAXHOST];
	char readBuf[MAX_BUF];

} CLIENT;

typedef struct response{
	char response_line[RESP_LINE_LEN];
	char **headers;
} RESPONSE;


CLIENT* get_client(int fd);

ssize_t ws_read(int fd, void* buf, size_t count);
ssize_t ws_write(int fd, void* buf, size_t count);

void wait_on_client(int servfd);
int add_client(int fd, struct sockaddr *addr, socklen_t *socklen);
void drop_client(int fd);

void serve_resource(CLIENT* cl);

/*----------------------------------------------------- log header ---------------------------------------------*/

#define INFO 0
#define WARN 1
#define ERR 2

void ws_log(FILE* fp, int mode, char *fmt, ...);
void errExit(FILE *fp, char *str);

ssize_t ws_read(int fd, void *buf, size_t count);
ssize_t ws_write(int fd, void *buf, size_t count);

/*----------------------------------------------------- error_code header ---------------------------------------------*/

void send_200(int fd, off_t len);
void send_400(int fd);
void send_404(int fd);
void send_411(int fd);
void send_413(int fd);		/* Request too large */
void send_414(int fd);		/* URI too large */
void send_500(int fd);
void send_501(int fd);
void send_505(int fd);


/*----------------------------------------------------- parse header ---------------------------------------------*/

#define GET 0
#define HEAD 1
#define POST 2

char *parse_url(char *str);
int parse_request_line(char *str, int fd);
int parse_header(char *str, int fd);
int parse_request(char *str, int fd);

/*----------------------------------------------------- end header ---------------------------------------------*/
#endif
