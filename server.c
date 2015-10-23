#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, cli_addr;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr;
	int n;
	int s;
	if (argc < 2)
	{
		fprintf(stderr,"Error, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("Error opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd,
		(struct sockaddr *) &cli_addr,
		&clilen);
	if (newsockfd < 0)
		error("ERROR on accept");
	bzero(buffer,256);
	
	n = read(newsockfd,buffer,255);
	if(n<0)
	        error("ERROR reading from socket");
	printf("Here is the message: %s\n",buffer);
	n = write(newsockfd,"I got your message",18);

	//get
	s=getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF,(struct sockaddr *) &cli_addr, &clilen);
	
	printf("getsockopt = %d\n", cli_addr);
	if(cli_addr > 0)
		printf("already set",cli_addr);
	else if(cli_addr < 0)
		printf("Error!,error number",cli_addr);
	else
		printf("elseeeeee",cli_addr);

	//set
	cli_addr = 98304;
	
	
	s = setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (struct sockaddr *) &cli_addr, sizeof(clilen));
	printf("SetSockopt = %d\n",cli_addr);

	return 0;

}
