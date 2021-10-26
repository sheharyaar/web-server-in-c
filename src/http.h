/* Custom functions return 0 in failure else non zero */
#ifndef HTTP_H
#define HTTP_H

#include "server.h"
#include "log.h"


#define MAX_BUF 1024
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


CLIENT* get_client(int fd);

ssize_t ws_read(int fd, void* buf, size_t count);
ssize_t ws_write(int fd, void* buf, size_t count);

void wait_on_client(int servfd);
int add_client(int fd, struct sockaddr *addr, socklen_t *socklen);
void drop_client(int fd);

void send_400(int fd);
void send_404(int fd);
void send_413(int fd);		/* Request too large */
void send_414(int fd);		/* URI too large */
void send_500(int fd);
void send_501(int fd);

void serve_resource(CLIENT* cl);



#endif
