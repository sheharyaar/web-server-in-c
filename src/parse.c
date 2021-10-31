#include "server.h"

/* TODO : Read URL Rfc and create a better parser
 *	Till now assume valid URI
 */

char *parse_url(char *str)
{
	char *res = strstr(str,"://");
	if(res !=NULL){
		/* absolute URI */
		res+=3;
		while(*res != '/'){
			++res;
		}

		return res;

	}
	else{
		res = str;
	}
	
	char *link = malloc((MAX_URI_LEN+2)*sizeof(char));
	if(link == NULL)
		return NULL;

	strcpy(link,"../www");
	strcpy(link+strlen(link),res);
	
	printf("DEBUG: LINK -- %s\n",link);

	if(strcmp(link,"../www/") == 0){
		strcpy(link+strlen(link),"index.html");
	}
	printf("DEBUG: LINK -- %s\n",link);

	return link;
}

/* Return error code on error or -1 on system error  */

int parse_request_line(char *str, int fd)
{
	char *str1, *save, *res;
	int i;
	int method;

	for(str1 = str, i = 0 ; ;str1=NULL, ++i ){

		res = strtok_r(str1," ",&save);

		if(res== NULL)
			break;

		if(i==0){	/* Method */
			if(strcmp("GET",res) == 0){
				method = GET;
			}
			else if(strcmp("HEAD",res) == 0){
				method = HEAD;
			}else{
				send_501(fd);
				return 501;
			}

			/* debugging */
			printf("Method : %s\n%d\n",res,method);
		}
	
		if(i == 1){ 	/* Url */

			if(strlen(res) > MAX_URI_LEN ){
				send_414(fd);
				return 414;

			}
			char *ret = parse_url(res);
			if(ret == NULL){
				send_500(fd);
				return 500;
			}

			printf("Url : %s\n",ret);

			struct stat statbuf;
			if(lstat(ret,&statbuf) == -1){
				send_404(fd);
				return 404;
			}

			off_t content_len = statbuf.st_size;
			
			send_200(fd,content_len);
			
			if(method == GET){
				int file = open(ret,O_RDONLY);
				if(file == -1){
					send_500(fd);
					return 500;
				}
				sendfile(fd,file,0,content_len);
				close(file);
			}
			
			
			/* TODO : Check for returned error and send error code 
			 *		- check for "/" and append base
			 *
			 * */
			/* debugging */
			printf("URL : %s\n",ret);

			return 200;
		}

		if(i == 2){	/* HTTP version */
			if(strstr(res,"1.1")==NULL){
				send_505(fd);
				return 505;
			}

			/* debugging */
			printf("HTTP Version : Correct\n");
		}
	}

	return 0;
}

/* Parses the header lines and returns 0 on success, else error codes or -1 for system
 * error 
 *
 *	Request header format:
 *		request-line
 *		request-headers CRLF
 *		CRLF CRLF
 *
 *	Request line:
 *		Method SP Request-URI SP HTTP-Version CRLF    -->
 *								Err 501 Not implemented
 *
 *	Request-URI:
 *		"*" | absoluteURI | abs_path | authority -->
 *								Err 414 Long URI
 *
 *	Error Code here:
 *		400 Bad Request
 *		404 not found
 *		411 Length required
 *		413 Paylaod Too large
 *
 */
int parse_request(char *request, int fd)
{
	char *str1, *res, *save;
	int i;
	
	/* Splitting the header and the body	*/
	char *str = strtok(request,"\r\n\r\n");
	if(str == NULL)
		return 400;
	
	/* Splitting individual lines */
	for(i=0,str1 = str; ; str1=NULL,++i){
		res = strtok_r(str1,"\r\n",&save);
		if(res == NULL)
			break;

		printf("Line : %s\n",res);

		if(i == 0){	/* Parsing the request-line */

			int ret = parse_request_line(res,fd);

			printf("Returned Value : %d\n",ret);

			if(ret!=0)
				return ret;
		}else{
			/* Parse the message headers */

		}
	}

	return 0;
}
