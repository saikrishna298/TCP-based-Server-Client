#include"header.h"
int main( int argc, char *argv[] )
{
	int     sd, ret_val = 0;
	struct  sockaddr_in serveraddress;
	char buffer[BUFSIZE];
	int index;
	sd = socket( PF_INET, SOCK_STREAM, 0 );
	if( sd < 0 ) {
		perror( "socket" );
		exit( 1 );
	}

	if (argv[1] == NULL ) {
		printf ("PL specfiy the IP address of the server. \n");
		exit(0);
	}

	if (argv[2] == NULL ) {
		printf ("PL specfiy the port number for the server. \n");
		exit(0);
	}

	if (argv[3] == NULL) {
		printf ("PL specify the connection type for the server. \n");
		exit(0);
	}//else strncpy(connection_type,argv[3],strlen(argv[3]));


	if (argv[4] == NULL ) {
		printf ("PL specfiy the filename \n");
		exit(0);
	}


	memset(&serveraddress,0,sizeof(struct sockaddr_in));
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(atoi(argv[2]));
	serveraddress.sin_addr.s_addr = inet_addr(argv[1]);

	if (connect(sd,(struct sockaddr *)&serveraddress,sizeof(serveraddress)) < 0)
	{
		perror("cONNECT ERROR\n");
		exit(1);
	}

	index = 0;
	(void)sprintf(buffer,"GET /%s HTTP/1.1\r\nHost: %s:%s\r\n", argv[4], argv[1], argv[2]);
	(void)write(sd,buffer,strlen(buffer));

	printf("%s\n",buffer);

	shutdown(sd,SHUT_WR); /*closing the write end*/
	index =0;
	while (1)
	{ 
	         
         	ret_val = read(sd,buffer,strlen(buffer));
                write(1,buffer,strlen(buffer));
                memset (&buffer[0],0,BUFSIZE);
	}


	close(sd);
	return 0;
}
