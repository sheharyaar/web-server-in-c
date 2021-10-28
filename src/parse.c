#include "server.h"
	
/* Returns the url */
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
	
	return str;
}

/* Return error code on error or -1 on system error 
 * 
 * TODO : Since we are returning only error code we need to verify if url exists or not
 * 	  Need to parse hex values in the url
 * 	  If method is head then dont send resource else send resource
 */

int parse_request_line(char *str)
{
	char *str1, *save, *res;
	int i;

	for(str1 = str, i = 0 ; ;str1=NULL, ++i ){

		res = strtok_r(str1," ",&save);

		if(res== NULL)
			break;

		if(i==0){	/* Method */
			if(strcmp("GET",res) == 0){

			}
			else if(strcmp("HEAD",res) == 0){

			}else{
				return 501;
			}

			/* debugging */
			printf("Method : %s\n",res);
		}
	
		if(i == 1){ 	/* Url */

			char *ret = parse_url(res);
			if(strlen(ret) > MAX_URI_LEN ){
				return 414;

			}
			/* TODO : Check for returned error and send error code 
			 *		- check for "/" and append base
			 *
			 * */
			/* debugging */
			printf("URL : %s\n",ret);
		}

		if(i == 2){	/* HTTP version */
			if(strstr(res,"1.1")==NULL)
				return 505;

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
 *								Err 405 Method not allowed
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
int parse_request(char *request)
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

			int ret = parse_request_line(res);

			printf("Returned Value : %d\n",ret);

			if(ret!=0)
				return ret;
		}else{
			/* Parse the message headers */

		}
	}

	return 0;
}
