#include "server.h"
#define DEBUG(x) fprintf(stdout,"REACHED HERE! : %d\n", (x) );


/* Import global variables from server.c*/
extern FILE* serverfp;
extern FILE* clientfp;


/* Client root node */
CLIENT *root = NULL;


/* fd set for elect */
fd_set masterset;
int maxfd = 0;


/* Function to get client from the list */
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
	maxfd = servfd;
	fd_set currentset;
	FD_ZERO(&masterset);
	FD_SET(servfd,&masterset);
	
	do{
		memcpy(&currentset, &masterset, sizeof(masterset));

		if(select(maxfd+1,&currentset,NULL,NULL,NULL) == -1){
			server_log(ERR,"select");
			exit(EXIT_SUCCESS);
		}
	
		/* Loop to check available fds */
		for( int i = 0 ; i <= maxfd ; i++ ){
			
			if(FD_ISSET(i,&currentset) != 0){
			
				if(i == servfd ){ /* Server is ready to accept more connections */	

					struct sockaddr client_addr;
					socklen_t client_addr_len = sizeof(client_addr);		

					int clientfd = accept(servfd, &client_addr, &client_addr_len);

					if(clientfd == -1 && errno!=EWOULDBLOCK && errno!=EAGAIN){
						server_log(ERR,"Error in accept : %s",strerror(errno));
					}else{
						if(clientfd > maxfd )		/* set the maxfd to the maximum obtaineed fd */
							maxfd = clientfd;

						FD_SET(clientfd,&masterset);

						/* store client data */
						if(add_client(clientfd,&client_addr, &client_addr_len) == 0){
							server_log(ERR,"Error in add_client(), dropping client");
							send_code(500,"NULL",clientfd,1,0);
						}	
						
						CLIENT* client = get_client(clientfd);

						server_log(INFO,"Client Connected, IP : %s",client->host);
						client_log(client->host,INFO,"Client Connected, IP : %s",client->host);

					}

				}else{
					
					/* Snippet to get request or serve data, cant't apply select to write on clientfd
					 * Beecause it will write before even client finishes sending the request
					 * */
					CLIENT* cl = get_client(i);
					serve_resource(cl);
								
				}

			}/* End of checking if i exists in returned set */

		} /* End of looping through the returned set */


	}while(1);

}

int add_client(int cfd, struct sockaddr *addr, socklen_t *socklen)
{
	/* Add client to client list */
	CLIENT *temp;
	int res;

	temp = malloc(sizeof(CLIENT));
	if(temp == NULL){
		server_log(ERR,"Error in malloc");
		return 0;
	}

	temp->cfd = cfd;
	temp->client_next = NULL;
	temp->socklen = *socklen;
	temp->addr = *addr;

	/* Get host string*/
	res = getnameinfo(&(temp->addr),temp->socklen,temp->host,NI_MAXHOST,NULL,0,NI_NUMERICHOST);
	if(res != 0){
		strcpy(temp->host,"NULL");
		char *err = (char *)gai_strerror(res);
		server_log(WARN,err);
	}

	temp->client_next = root;
	root = temp;

	return 1;
}

void drop_client(int fd)
{
	/* Remove client from list */
	CLIENT **rpp = &root;
	CLIENT *rp = root;

	while(rp!=NULL){
		if(rp->cfd == fd){
			*rpp = rp->client_next;
			free(rp);
			break;
		}
		rpp = &(rp->client_next);
		rp = rp->client_next;
	}


	/* Remove client from fd set */
	FD_CLR(fd,&masterset);

	/* Close the cinnection
	 * TODO:
	 * 	Check for RST received by client to prevent premature closing
	 */
	close(fd);

	/* If the closed fd is max then decrease for select() call */
	if(fd == maxfd){
		while(FD_ISSET(maxfd,&masterset) == 0){
			maxfd--;
		}
	}
}

void serve_resource(CLIENT* cl)
{
	int fd = cl->cfd;
	memset(&cl->readBuf,0,MAX_BUF);
	cl->readData = ws_read(fd,cl->readBuf,MAX_BUF);

	if(cl->readData == 0){		/* If the client closes connection read gives 0 and write gives EPIPE*/
		drop_client(fd);
		return ;
	}

	client_log(cl->host,INFO,cl->readBuf);

	parse_request(cl->host, cl->readBuf,fd);
}

