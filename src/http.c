/* TODO:
 *	
 *	classify requests as good or bad and echo back the good requests and 
 *	for bad requests send 400
 *
 * 	Implement Content length
 *
 * 	- Improve log
 * 	- implement print_clients()
 * 	- Now parse requests
 * 	- Check if linked list working or not
 */
#include "http.h"
#define DEBUG(x) fprintf(stdout,"REACHED HERE! : %d\n", (x) );


extern FILE* fp;
CLIENT *root;

fd_set masterset;
int maxfd = 0;

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

		if(select(maxfd+1,&currentset,NULL,NULL,NULL) == -1)
			errExit(fp,"select");
	
		for( int i = 0 ; i <= maxfd ; i++ ){
			
			if(FD_ISSET(i,&currentset) != 0){
			
				if(i == servfd ){ /* Server is ready to accept more connections */	

					struct sockaddr client_addr;
					socklen_t client_addr_len = sizeof(client_addr);		

					int clientfd = accept(servfd, &client_addr, &client_addr_len);

					if(clientfd == -1 && errno!=EWOULDBLOCK && errno!=EAGAIN){
						ws_log(fp,ERR,"Error in accept : %s",strerror(errno));
					}else{
						if(clientfd > maxfd )		/* set the maxfd to the maximum obtaineed fd */
							maxfd = clientfd;

						FD_SET(clientfd,&masterset);


						/* store client data */
						if(add_client(clientfd,&client_addr, &client_addr_len) == 0){
							ws_log(fp,ERR,"Error in add_client(), dropping client");
							send_500(i);
						}	
						
						CLIENT* client = get_client(clientfd);
						ws_log(fp,INFO,"Client Connected IP :");
						ws_log(fp,INFO,client->host);

					}

				}else{
					
					/* Snippet to get request or serve data, cant't apply select to write on clientfd
					 * Beecause it will write before even client finishes sending the request
					 * */
					CLIENT* cl = get_client(i);
					serve_resource(cl);
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
	res = getnameinfo(&(temp->addr),temp->socklen,temp->host,NI_MAXHOST,NULL,0,NI_NUMERICHOST);
	if(res != 0){
		temp->host[0] = 'N';
		temp->host[1] = 'U';
		temp->host[2] = 'L';
		temp->host[3] = 'L';
		temp->host[4] = '\0';
		char *err = (char *)gai_strerror(res);
		ws_log(fp,WARN,err);
	}

	temp->client_next = root;
	root = temp;

	return 1;
}

void drop_client(int fd)
{
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


	FD_CLR(fd,&masterset);
	close(fd);

	if(fd == maxfd){
		while(FD_ISSET(maxfd,&masterset) == 0){
			maxfd--;
		}
	}
}

void serve_resource(CLIENT* cl)
{
	int fd = cl->cfd;
	cl->readData = ws_read(fd,cl->readBuf,MAX_BUF);

	if(cl->readData == 0){		/* If the client closes connection read gives 0 and write gives EPIPE*/
		drop_client(fd);
		return ;
	}

	ws_log(fp,INFO,cl->readBuf);
	
	send_413(cl->cfd);

}

void send_500(int fd)
{
	char buf[] = "HTTP/1.1 500 Internal Server Error\r\nConnection: close\r\n\r\n";
	ws_write(fd,buf,sizeof(buf));
	ws_log(fp,INFO,buf);
	drop_client(fd);
}

void send_413(int fd)
{
	/* Construct message */
	char buffer[MAX_BUF];
	snprintf(buffer,MAX_BUF,"HTTP/1.1 413 Request Size Too Large\r\nConnection: close\r\n\r\n");
	ws_write(fd,buffer,strlen(buffer));

	drop_client(fd);
}

ssize_t ws_read(int fd, void *buf, size_t count)
{
	ssize_t readNum = read(fd,buf,count);
	if(readNum == -1 && errno!= EWOULDBLOCK && errno!= EAGAIN){
		ws_log(fp,ERR,"read");
	}

	return readNum;
}

ssize_t ws_write(int fd, void *buf, size_t count)
{
	ssize_t writeNum = write(fd,buf,count);
	if(writeNum == -1 && errno!= EWOULDBLOCK && errno!= EAGAIN){
		ws_log(fp,ERR,"read");
	}

	return writeNum;
}
