#include "server.h"

extern FILE* serverfp;
extern FILE* clientfp;
extern pid_t server_pid;

/* Log format :
 * 	[TYPE] [CLIENT_IP] [TIME] [REQ] [RESPONSE_CODE] [SIZE] [USER-AGENT]
 */

void server_log(int mode, char *fmt,...)
{
	setbuf(serverfp,0);	// immediately flush the output

	time_t t;
	time(&t);
	struct tm *t1 = gmtime(&t);

	char buf[MAX_LOG_BUF];
	char tag[5];

	if(mode == INFO)
		strncpy(tag,"INFO",5);

	if(mode == WARN)
		strncpy(tag,"WARN",5);
	
	if(mode == ERR)
		strncpy(tag,"ERR",5);

	sprintf(buf,"[%d][%02d-%02d-%04d %02d:%02d:%02d][%s] : ",server_pid,t1->tm_mday,t1->tm_mon+1,t1->tm_year+1900,t1->tm_hour,t1->tm_min,t1->tm_sec,tag);

	size_t buf_len = strlen(buf);

	va_list ap;
	va_start(ap,fmt);
	vsnprintf(buf+buf_len,MAX_LOG_BUF-(1+buf_len),fmt,ap);
	va_end(ap);

	fprintf(serverfp,"%s\n",buf);
}

void client_log(char *host, int mode, char *fmt,...)
{
	/* TODO : Add User agent info in the log and request info
	 */
	setbuf(clientfp,0);	// immediately flush the output

	time_t t;
	time(&t);
	struct tm *t1 = gmtime(&t);

	char buf[MAX_LOG_BUF];
	char tag[5];

	if(mode == INFO)
		strncpy(tag,"INFO",5);

	if(mode == WARN)
		strncpy(tag,"WARN",5);
	
	if(mode == ERR)
		strncpy(tag,"ERR",5);

	sprintf(buf,"[%s][%02d-%02d-%04d %02d:%02d:%02d][%s] : ",host,t1->tm_mday,t1->tm_mon+1,t1->tm_year+1900,t1->tm_hour,t1->tm_min,t1->tm_sec,tag);

	size_t buf_len = strlen(buf);

	va_list ap;
	va_start(ap,fmt);
	vsnprintf(buf+buf_len,MAX_LOG_BUF-(1+buf_len),fmt,ap);
	va_end(ap);

	fprintf(clientfp,"%s\n",buf);
}

ssize_t ws_read(int fd, void *buf, size_t count)
{
	ssize_t readNum = read(fd,buf,count);
	if(readNum == -1 && errno!= EWOULDBLOCK && errno!= EAGAIN){
		server_log(ERR,"read");
	}

	return readNum;
}

ssize_t ws_write(int fd, void *buf, size_t count)
{
	ssize_t writeNum = write(fd,buf,count);
	if(writeNum == -1 && errno!= EWOULDBLOCK && errno!= EAGAIN){
		server_log(ERR,"read");
	}

	return writeNum;
}
