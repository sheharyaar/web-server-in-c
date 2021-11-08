/*
 * Arguments to main :
 * 	- program name
 * 	- port
 * 	- logfile
 */
#include "server.h"

pid_t server_pid;
FILE *serverfp = NULL;
FILE *clientfp = NULL;
extern CLIENT* root;

static void handler(int);

int main(int argc, char *argv[])
{

	// Usage Error
	if(argc!=5){
		fprintf(stderr,"Usage :\n%s host port server-logfile client-logfile\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	// Open serverfile and logfile
	serverfp = fopen(argv[3],"a");
	if(serverfp == NULL){
		fprintf(stderr,"Error in opening server-logfile");
		exit(EXIT_SUCCESS);
	}

	clientfp = fopen(argv[4],"a");
	if(clientfp == NULL){
		fprintf(stderr,"Error in opening client-logfile");
		exit(EXIT_FAILURE);
	}

	// server PID
	server_pid = getpid();

	// Mask SIGINT signal to force use of exit handler
	struct sigaction sa;
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	
	if(sigaction(SIGINT,&sa,NULL) == -1){
		server_log(ERR,"sigaction");
		exit(EXIT_FAILURE);
	}

	// Exit handler
	if(atexit(cleanup)!=0){
		server_log(ERR,"atextit");
		exit(EXIT_FAILURE);
	}

	// Create server
	int servfd = init_server(argv[1],argv[2], AF_INET, IPPROTO_TCP, SOCK_STREAM);

	// Wait on client
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

	if(getaddrinfo(host,service,&hints,&res)!=0){
		server_log(ERR,"getaddrinfo");
		exit(EXIT_FAILURE);
	}

	/* Get Socket, apply NONBLOCK and bind to it */
	for(rp = res; rp!=NULL; rp=res->ai_next){
		servfd = socket(rp->ai_family,rp->ai_socktype | SOCK_NONBLOCK,rp->ai_protocol);
		if(servfd== -1)
			continue;

		int val = 1;

		/* Set SO_REUSEADDR */
		if(setsockopt(servfd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) == -1){
			server_log(ERR,"setsockopt-SO_REUSEADDR");
			exit(EXIT_FAILURE);
		}

		if(bind(servfd,rp->ai_addr,rp->ai_addrlen)==0){
			break;
		}
		
	}
	
	/* No address to bind */
	if(rp == NULL){
		server_log(ERR,"bind - could not bind to any address");
		exit(EXIT_FAILURE);
	}
	
	server_log(INFO,"Successfully bind to socket");


	/* Listen on server */
	if(listen(servfd,MAX_CLIENT) == -1 ){
		server_log(ERR,"listen");
		exit(EXIT_FAILURE);
	}
	
	server_log(INFO,"Listening on socket");

	return servfd;
}	

void cleanup(void)
{
	/* Free the list of client on exit */
	server_log(INFO,"Freeing client list");
	CLIENT *temp;
	while(root!=NULL){
		temp = root;
		root = root->client_next;
		free(temp);
	}
	server_log(INFO,"CLient list freed");
}

static void handler(int sig)
{
	exit(EXIT_FAILURE);
}

