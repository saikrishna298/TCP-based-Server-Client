#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<pthread.h> 

#define MYPORT 4121
#define BCKLOG 5
#define SERVER_IP "129.79.247.5"
#define BUFSIZE 8096
#define VERSION 23
#define FORBIDDEN 403

typedef struct snode
{
        char data[64];
        struct snode *next;
}snode;
int push(snode **top,char * item);

char* pop(snode **top);
//the thread function
void *connection_handler(void *);
 
