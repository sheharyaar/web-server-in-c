#include "server.h"

extern FILE* fp;

const char *day[]={
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"
};

const char *month[] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};

static char *err_time(char *buf){
	time_t t;
	time(&t);
	struct tm *gmt = gmtime(&t);
	snprintf(buf,30,"%s, %02d %s %04d %02d:%02d:%02d GMT",day[gmt->tm_wday],gmt->tm_mday,month[gmt->tm_mon],1900+gmt->tm_year,
			gmt->tm_hour,gmt->tm_min,gmt->tm_sec);

	return buf;
}

void send_200(int fd, off_t len)
{
	char buffer[MAX_BUF];
	char timebuf[30];
	err_time(timebuf);
	snprintf(buffer,MAX_BUF,"HTTP/1.1 200 OK\r\nDate: %s\r\n"
			"Content-Length: %ld\r\n\r\n",timebuf,len);
	ws_write(fd,buffer,strlen(buffer));
	ws_log(fp,INFO,"Sent 200");

}

void send_400(int fd)
{
	char buffer[MAX_BUF];
	char timebuf[30];
	err_time(timebuf);
	snprintf(buffer,MAX_BUF,"HTTP/1.1 400 Bad Request\r\nDate: %s\r\n"
			"Connection: close\r\nContent-Length: 15\r\n\r\n400 Bad Request",timebuf);
	ws_write(fd,buffer,strlen(buffer));
	ws_log(fp,INFO,"Sent 400");
	drop_client(fd);

}

void send_404(int fd)
{
	/* Construct message */
	char buffer[MAX_BUF];
	char timebuf[30];
	err_time(timebuf);
	snprintf(buffer,MAX_BUF,"HTTP/1.1 404 Not Found\r\nDate: %s\r\n"
			"Connection: close\r\nContent-Length: 13\r\n\r\n404 Not Found",timebuf);
	ws_write(fd,buffer,strlen(buffer));
	ws_log(fp,INFO,"Sent 404");
	drop_client(fd);
}

void send_411(int fd)
{
	/* Construct message */
	char buffer[MAX_BUF];
	char timebuf[30];
	err_time(timebuf);
	snprintf(buffer,MAX_BUF,"HTTP/1.1 411 Length Required\r\nDate: %s\r\n"
			"Connection: close\r\nContent-Length: 19\r\n\r\n411 Length Required",timebuf);
	ws_write(fd,buffer,strlen(buffer));
	ws_log(fp,INFO,"Sent 411");
	drop_client(fd);
}

void send_413(int fd)
{
	/* Construct message */
	char buffer[MAX_BUF];
	char timebuf[30];
	err_time(timebuf);
	snprintf(buffer,MAX_BUF,"HTTP/1.1 413 Request Size Too Large\r\nDate: %s\r\n"
			"Connection: close\r\nContent-Length: 22\r\n\r\nRequest Size Too Large",timebuf);
	ws_write(fd,buffer,strlen(buffer));
	ws_log(fp,INFO,"Sent 413");
	drop_client(fd);
}

void send_414(int fd)		/* URI too large */
{
	/* Construct message */
	char buffer[MAX_BUF];
	char timebuf[30];
	err_time(timebuf);
	snprintf(buffer,MAX_BUF,"HTTP/1.1 413 Request Size Too Large\r\nDate: %s\r\n"
			"Connection: close\r\nContent-Length: 22\r\n\r\nRequest Size Too Large",timebuf);
	ws_write(fd,buffer,strlen(buffer));
	ws_log(fp,INFO,"Sent 414");
	drop_client(fd);
}

void send_500(int fd)
{
	char buffer[MAX_BUF];
	char timebuf[30];
	err_time(timebuf);
	snprintf(buffer,MAX_BUF,"HTTP/1.1 500 Internal Server Error\r\nDate: %s\r\n"
			"Connection: close\r\nContent-Length: 25\r\n\r\n500 Internal Server Error",timebuf);
	ws_write(fd,buffer,strlen(buffer));
	ws_log(fp,INFO,"Sent 500");
	drop_client(fd);
}
void send_501(int fd)
{
	char buffer[MAX_BUF];
	char timebuf[30];
	err_time(timebuf);
	snprintf(buffer,MAX_BUF,"HTTP/1.1 501 Not Implemented\r\nDate: %s\r\n"
			"Connection: close\r\nContent-Length: 19\r\n\r\n501 Not Implemented",timebuf);
	ws_write(fd,buffer,strlen(buffer));
	ws_log(fp,INFO,"Sent 501");
	drop_client(fd);
}

void send_505(int fd)
{
	char buffer[MAX_BUF];
	char timebuf[30];
	err_time(timebuf);
	snprintf(buffer,MAX_BUF,"HTTP/1.1 505 HTTP Version Not Supported\r\nDate: %s\r\n"
			"Connection: close\r\nContent-Length: 25\r\n\r\n505 HTTP Version Not Supported",timebuf);
	ws_write(fd,buffer,strlen(buffer));
	ws_log(fp,INFO,"Sent 505");
	drop_client(fd);
}

