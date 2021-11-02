#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

pid_t server_pid;

void custom_func(int id,char *fmt,...) 
{

	time_t t;
	time(&t);

	struct tm *t1 = gmtime(&t);

	char buf[256];
	sprintf(buf,"[%d][%02d-%02d-%04d %02d:%02d:%02d]- ",server_pid,t1->tm_mday,t1->tm_mon+1,t1->tm_year+1900,t1->tm_hour,t1->tm_min,t1->tm_sec);

	va_list ap;
	va_start(ap,fmt);
	vsnprintf(buf+strlen(buf),256-strlen(buf),fmt,ap);
	va_end(ap);
	fprintf(stdout,"%s\n",buf);
}

int main()
{
	server_pid = getpid();
	custom_func(1,"Hello World from a function %s which tells %d\n","Telco",3);
	return 0;
}
