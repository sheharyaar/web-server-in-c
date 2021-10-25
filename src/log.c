#include "log.h"

/* Log format :
 * 	[TYPE] [CLIENT_IP] [TIME] [REQ] [RESPONSE_CODE] [SIZE] [USER-AGENT]
 */
void ws_log(FILE *fp, int mode, char *fmt, ...)
{
	fp = stdout;
	setbuf(fp,0);
	if(mode == INFO)
		fprintf(fp,"[INFO]\t");

	if(mode == WARN)
		fprintf(fp,"[WARN]\t");
	
	if(mode == ERR)
		fprintf(fp,"[ERR]\t");

	printf("%s=%d : %s\n",fmt,errno,strerror(errno));
}

void errExit(FILE *fp, char *str){
	fprintf(fp,"Error in %s : %s\n",str,strerror(errno));
	exit(EXIT_FAILURE);
}
