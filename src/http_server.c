#include"header.h"

int main(int argc, char *argv[])
{

	int listensocket,connectionsocket;
	struct  sockaddr_in
		serveraddress,cliaddr;
	char *server_ip = SERVER_IP; 
	int binderror;
	int index;
	socklen_t len;
	char buf[1024];
	char readbuf[BUFSIZE];
	pthread_t thread_id;	
	listensocket = socket(AF_INET, SOCK_STREAM, 0 );
	if (listensocket < 0 )
	{
		perror("socket" );
		exit(1 );
	}

	memset(&serveraddress, 0, sizeof(serveraddress) );
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(MYPORT);/*PORT NO*/
	serveraddress.sin_addr.s_addr = inet_addr(server_ip);/*ADDRESS*/
	binderror=bind(listensocket,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
	if (-1 == binderror)
	{
		perror("BIND !!!!!!!");
		exit(1);
	}
	listen(listensocket,5);

	/*Beginning of the Main Server Processing Loop*/
	memset(buf,0,sizeof(buf));
	memset(readbuf,0,sizeof(readbuf));

	printf("Server:I am waiting-----\n");
	len=sizeof(cliaddr);
	while ((connectionsocket = (accept(listensocket,
						(struct sockaddr*)&cliaddr,&len))))
	{
		if (connectionsocket < 0)
		{
			if (errno == EINTR)
				printf("Interrupted system call ??");
			continue;
		}
		printf("Connection from %s\n",
				inet_ntop(AF_INET,&cliaddr.sin_addr,buf,sizeof(buf)));
		//	sleep(1);	/* allow socket to drain before signalling the socket is closed */
		if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &connectionsocket) < 0)
		{
			perror("could not create thread");
			return 1;
		}

		puts("Handler assigned");
	}

	if (connectionsocket < 0)
	{
		if (errno == EINTR)
			printf("Interrupted system call ");
	}

	close(connectionsocket);

	shutdown(connectionsocket,SHUT_RD);





	return 0;
}

void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_chars,i,file_fd;
	long len;
	char readbuf[BUFSIZE]; ;     
	char * fstr = "NULL"; 
	//Send some messages to the client
	printf ("Greetings! I am your connection handler\n");


	read_chars = read(sock,readbuf,BUFSIZE);

	//ret =read(fd,buffer,BUFSIZE); 	/* read Web request in one go */
	if(read_chars == 0 || read_chars == -1) {	/* read failure stop now */
		printf ("FORBIDDEN: failed to read browser request");
	}
	if(read_chars > 0 && read_chars < BUFSIZE)	/* return code is valid chars */
		readbuf[read_chars]=0;		/* terminate the buffer */

	else readbuf[0]=0;

	for(i=0;i<read_chars;i++)	/* remove CF and LF characters */
		if(readbuf[i] == '\r' || readbuf[i] == '\n')
			readbuf[i]='*';
	//logger(LOG,"request",buffer,hit);
	if( strncmp(readbuf,"GET ",4) && strncmp(readbuf,"get ",4) ) {
		printf("FORBIDDEN : Only simple GET operation supported");
	}
	for(i=4;i<BUFSIZE;i++) { /* null terminate after the second space to ignore extra stuff */
		if(readbuf[i] == ' ') { /* string is "GET URL " +lots of other stuff */
			readbuf[i] = 0;
			break;
		}
	}
	//for(j=0;j<i-1;j++) 	/* check for illegal parent directory use .. */
	//	if(readbuf[j] == '.' && readbuf[j+1] == '.') {
	//		logger(FORBIDDEN,"Parent directory (..) path names not supported",buffer,fd);
	//	}
	if( !strncmp(&readbuf[0],"GET /\0",6) || !strncmp(&readbuf[0],"get /\0",6) ) /* convert no filename to index file */
		(void)strcpy(readbuf,"GET /index.html");

	/* work out the file type and check we support it */
	/*buflen=strlen(readbuf);
	  fstr = (char *)0;
	  for(i=0;extensions[i].ext != 0;i++) {
	  len = strlen(extensions[i].ext);
	  if( !strncmp(&buffer[buflen-len], extensions[i].ext, len)) {
	  fstr =extensions[i].filetype;
	  break;
	  }
	  }*/

	//if(fstr == 0) logger(FORBIDDEN,"file extension type not supported",buffer,fd);

	if(( file_fd = open(&readbuf[5],O_RDONLY)) == -1) {  /* open the file for reading */
		printf( "NOTFOUND: failed to open file");
	}
	//logger(LOG,"SEND",&buffer[5],hit);

	len = (long)lseek(file_fd, (off_t)0, SEEK_END); /* lseek to the file end to find the length */
	(void)lseek(file_fd, (off_t)0, SEEK_SET); /* lseek back to the file start ready for reading */
	(void)sprintf(readbuf,"HTTP/1.1 200 OK\nServer: nweb/%d.0\nContent-Length: %ld\nConnection: close\nContent-Type: %s\n\n", VERSION, len, fstr); /* Header + a blank line */
	//logger(LOG,"Header",buffer,hit);
	(void)write(sock,readbuf,strlen(readbuf));

	/* send file in 8KB block - last block may be smaller */
	while (	(read_chars = read(file_fd, readbuf, BUFSIZE)) > 0 ) {
		(void)write(sock,readbuf,read_chars);
	}

	//(void)write(sock,fstr,strlen(fstr));


	sleep(1);	/* allow socket to drain before signalling the socket is closed */
	close(sock);

	shutdown(sock,SHUT_RD);


	return 0;
} 

