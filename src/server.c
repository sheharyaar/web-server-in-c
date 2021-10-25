/*
 * Arguments to main :
 * 	- program name
 * 	- port
 * 	- logfile
 */
#include "server.h"
#include "http.h"

FILE *fp = NULL;

int main(int argc, char *argv[])
{

	if(argc!=3){
		fprintf(stderr,"Usage :\n%s port logfile\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	fp = fopen(argv[2],"a");
	if(fp == NULL)
		errExit(stderr,"opening logfile");

	int servfd = init_server("192.168.0.163",argv[1], AF_INET, IPPROTO_TCP, SOCK_STREAM);

	wait_on_client(servfd);
	
	exit(EXIT_SUCCESS);
}

int init_server(char *host, char *service, int family, int protocol, int socktype)
{	
	int servfd;
	struct addrinfo *rp,*res, hints;
	
	/* Zero out the hints memory */
	memset(&hints,0,sizeof(hints));
	hints.ai_family = family;
	hints.ai_protocol = protocol;
	hints.ai_socktype = socktype;

	if(getaddrinfo(host,service,&hints,&res)!=0)
		errExit(fp,"getaddrinfo");

	/* Get Socket, apply NONBLOCK and bind to it */

	for(rp = res; rp!=NULL; rp=res->ai_next){
		servfd = socket(rp->ai_family,rp->ai_socktype | SOCK_NONBLOCK,rp->ai_protocol);
		if(servfd== -1)
			continue;

		if(bind(servfd,rp->ai_addr,rp->ai_addrlen)==0){
			break;
		}
		
	}
	
	/* no address to bind */
	if(rp == NULL)
		errExit(fp,"bind - could not bind to any address");
	
	/* Set SO_REUSEADDR */
	if(setsockopt(servfd,SOL_SOCKET,SO_REUSEADDR,&(int){1},sizeof(int)) == -1)
		errExit(fp,"setsockopt-SO_REUSEADDR");

	/* Listen on server */
	if(listen(servfd,MAX_CLIENT) == -1 )
		errExit(fp,"listen");
	

	return servfd;
}	


