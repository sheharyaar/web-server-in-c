/*
 * Arguments to main :
 * 	- program name
 * 	- port
 * 	- logfile
 */
#include "server.h"

FILE *fp = NULL;
extern CLIENT* root;

static void handler(int);

int main(int argc, char *argv[])
{


	if(argc!=3){
		fprintf(stderr,"Usage :\n%s port logfile\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	/*
	if(chroot("../www/") == -1)
		errExit(stderr,"chroot");
	*/
	
	struct sigaction sa;
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	
	if(sigaction(SIGINT,&sa,NULL) == -1)
		errExit(fp,"sigaction");

	if(atexit(cleanup)!=0)
		errExit(stderr,"atextit");

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

		int val = 1;
		if(setsockopt(servfd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) == -1)
			errExit(fp,"setsockopt-SO_REUSEADDR");

		if(bind(servfd,rp->ai_addr,rp->ai_addrlen)==0){
			break;
		}
		
	}
	
	/* no address to bind */
	if(rp == NULL)
		errExit(fp,"bind - could not bind to any address");
	
	ws_log(fp,INFO,"Successfully bind to socket");

	/* Set SO_REUSEADDR */

	/* Listen on server */
	if(listen(servfd,MAX_CLIENT) == -1 )
		errExit(fp,"listen");
	
	ws_log(fp,INFO,"Listening on socket");

	return servfd;
}	

void cleanup(void)
{
	/* Free the list of client on exit */
	ws_log(fp,INFO,"Freeing client list");
	CLIENT *temp;
	while(root!=NULL){
		temp = root;
		root = root->client_next;
		free(temp);
	}
	ws_log(fp,INFO,"CLient list freed");
}

static void handler(int sig)
{
	exit(EXIT_FAILURE);
}

