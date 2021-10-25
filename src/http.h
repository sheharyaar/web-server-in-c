/* Custom functions return 0 in failure else non zero */
#ifndef HTTP_H
#define HTTP_H

#include "server.h"
#include "log.h"

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
	socklen_t socklen;
	void *buf;
	struct client *client_next;
	struct sockaddr addr;
	char host[NI_MAXHOST];

} CLIENT;

CLIENT* get_client(int fd);

void wait_on_client(int servfd);

int add_client(int fd, struct sockaddr *addr, socklen_t *socklen);
void drop_client(int fd);

void send_400();
void send_404();
void send_500(int fd);
void send_501();

void serve_resource();



#endif
