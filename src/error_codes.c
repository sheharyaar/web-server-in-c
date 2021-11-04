/*
 * Think about a response message structure 
 * 	Response Line
 * 	Date: Repsonse Time
 * 	Location: ???
 *	Content-Length: 
 *	Content-Type: 
 *	Body
 *
 */

/* Think about a request storage structure */

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
	snprintf(buf,DATE_LEN,"%s, %02d %s %04d %02d:%02d:%02d GMT",day[gmt->tm_wday],gmt->tm_mday,month[gmt->tm_mon],1900+gmt->tm_year,
			gmt->tm_hour,gmt->tm_min,gmt->tm_sec);

	return buf;
}

RESP_CODE resp_code_list[] = {
	{200,"OK"},
	{400,"Bad Request"},
	{404,"Not Found"},
	{411,"Length Required"},
	{413,"Request Entity Too Large"},
	{414,"URI Too Long"},
	{500,"Internal Server Error"},
	{501,"Not Implemented"},
	{505,"HTTP Version Not Supported"},
};

void send_code(int err, char *host, int fd, int close, size_t body)
{
	char buffer[RESP_LEN+1];
	char timebuf[DATE_LEN];
	err_time(timebuf);
	
	int flag = -1;
	for(int i = 0 ; i < (sizeof(resp_code_list)/sizeof(RESP_CODE)) ; i++){
		if(resp_code_list[i].code == err)
			flag = i;
	}

	if(flag == -1){
		server_log(ERR,"Error in finding response code");
		drop_client(fd);
		return;
	}

	snprintf(buffer,RESP_LEN,"HTTP/1.1 %d %s\r\nDate: %s\r\nServer: CUSTOM/1.0\r\n",err,resp_code_list[flag].str,timebuf);

	if(close)
		snprintf(buffer+strlen(buffer),RESP_LEN - strlen(buffer),"Connection: close\r\n");

	if(body == 0){
		if(err!=200){
			snprintf(buffer+strlen(buffer),RESP_LEN - strlen(buffer),"Content-Length: %lu\r\n",strlen(resp_code_list[flag].str) );
		}
	}else{
		snprintf(buffer+strlen(buffer),RESP_LEN - strlen(buffer),"Content-Length: %lu\r\n",body);
	}

	snprintf(buffer+strlen(buffer),RESP_LEN - strlen(buffer),"\r\n");
	
	if(body == 0 && err!=200)
		snprintf(buffer+strlen(buffer),RESP_LEN - strlen(buffer),"%s",resp_code_list[flag].str);

	ws_write(fd,buffer,strlen(buffer));

	client_log(host,INFO,"HTTP/1.1 %d %s",err,resp_code_list[flag].str);

	if(close)
		drop_client(fd);
}

