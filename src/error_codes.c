#include "server.h"

extern FILE* fp;

void send_500(int fd)
{
	char buf[] = "HTTP/1.1 500 Internal Server Error\r\nConnection: close\r\nContent-Length: 21\r\n\r\nInternal Server Error";
	ws_write(fd,buf,sizeof(buf));
	ws_log(fp,INFO,buf);
	drop_client(fd);
}

void send_413(int fd)
{
	/* Construct message */
	char buffer[MAX_BUF];
	snprintf(buffer,MAX_BUF,"HTTP/1.1 413 Request Size Too Large\r\nConnection: close\r\nContent-Length: 11\r\n\r\nBad Request");
	ws_write(fd,buffer,strlen(buffer));

	drop_client(fd);
}
