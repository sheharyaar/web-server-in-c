#include <stdio.h>
#include <stdlib.h>

typedef struct client{
	int cfd;
	struct client *client_next;
} CLIENT;

CLIENT *root = NULL;

void printList()
{
	CLIENT *rp = root;
	while(rp!=NULL){
		printf("%d\t",rp->cfd);
		rp = rp->client_next;
	}
	printf("\n");
}

void addElem(int num)
{
	CLIENT *temp;
	temp = malloc(sizeof(CLIENT));
	temp->cfd = num;
	temp->client_next = root;
	root = temp;
}

void deleteElem(int num){
	CLIENT **rpp = &root;
	CLIENT *rp = root;

	while(rp!=NULL){
		if(rp->cfd == num){
			*rpp = rp->client_next;
			free(rp);
			break;
		}
		rpp = &(rp->client_next);
		rp = rp->client_next;
	}
}

int main(){
	addElem(1);
	addElem(2);
	addElem(3);
	addElem(4);
	addElem(5);
	addElem(6);
	printList();
	deleteElem(6);
	printList();
	deleteElem(3);
	printList();
	deleteElem(1);
	printList();
	deleteElem(5);
	printList();

}
