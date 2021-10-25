/* TODO:
 * 	- Implement add_client()
 * 	- Implement drop_client()
 * 	- Improve log
 */
#include "http.h"

extern FILE* fp;
CLIENT *root;

CLIENT* get_client(int fd)
{
	CLIENT* rp = root;
	while(rp != NULL){
		if(rp->cfd == fd)
			return rp;

		rp = rp->client_next;
	}

	return NULL ;
}

void wait_on_client(int servfd)
{	
	int maxfd;
	fd_set masterset, currentset;
	FD_ZERO(&masterset);
	FD_SET(servfd,&masterset);
	
	do{
		memcpy(&currentset, &masterset, sizeof(masterset));

		if(select(maxfd+1,&currentset,NULL,NULL,NULL) == -1)
			errExit(fp,"select");
	
		for( int i = 0 ; i < maxfd ; i++ ){
			
			if(FD_ISSET(i,&currentset) != 0){
			
				if(i == servfd ){ /* Server is ready to accept more connections */	

					struct sockaddr client_addr;
					socklen_t client_addr_len;		

					int clientfd = accept(servfd, &client_addr, &client_addr_len);

					if(clientfd == -1){
						ws_log(fp,ERR,"error in accept : %s",strerror(errno));
					}else{
						if(clientfd > maxfd )		/* set the maxfd to the maximum obtaineed fd */
							maxfd = clientfd;

						FD_SET(clientfd,&masterset);

						ws_log(fp,INFO,"Client Connected");

						/* store client data */
						if(add_client(clientfd,&client_addr, &client_addr_len) == 0){
							ws_log(fp,ERR,"Error in add_client(), dropping client");
							send_500(i);
						}	
					}

				}else{
					
					/* Snippet to get request or serve data, cant't apply select to write on clientfd
					 * Beecause it will write before even client finishes sending the request
					 * */
					//CLIENT *cl;
					//cl = get_client(i);
					
					char buf[] = "hehe bye!\n";
					send(i,buf,sizeof(buf),0);

					close(i);
					/* If the client drops reduce the maxfd to
					 * the next highest available fd and remove from the maxset*/
								
				}

			}/* End of checking if i exists in returned set */

		} /* End of looping through the returned set */


	}while(1);

}

int add_client(int cfd, struct sockaddr *addr, socklen_t *socklen)
{
	/* Implement adding to linked list
	 * Call getnameinfo to add host and server
	 */
	CLIENT *temp;
	int res;

	temp = malloc(sizeof(CLIENT));
	if(temp == NULL){
		ws_log(fp,ERR,"Error in malloc");
		return 0;
	}

	temp->cfd = cfd;
	temp->client_next = NULL;
	temp->socklen = *socklen;
	temp->addr = *addr;
	temp->buf = NULL;

	res = getnameinfo(&(temp->addr),temp->socklen,temp->host,NI_MAXHOST,NULL,0,NI_NAMEREQD | NI_NUMERICHOST);
	if(res != 0){
		temp->host[0] = 'E';
		ws_log(fp,WARN,"%s",gai_strerror(res));
	}

	temp->client_next = root;
	root = temp;

	return 1;
}

/* TODO: Complete this function */
void drop_client(int fd)
{
	CLIENT *rp = root;

	while(rp!=NULL){
		if(rp->cfd == fd){
			break;
		}
	}
	
	close(fd);
}

/* TODO: Complete this function */
void send_500(int fd)
{
	char buf[] = "HTTP/1.1 500 Internal Server Error\r\nConnection: Closed\r\n\r\n";
	send(fd,buf,sizeof(buf),0);
	ws_log(fp,INFO,buf);
	drop_client(fd);
}
