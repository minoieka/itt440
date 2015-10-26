#include<stdio.h> 
#include<stdlib.h>
#include<sys/types.h> 
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h> 
#include<errno.h> 
#include<unistd.h>
#include<netdb.h>

void error(char *msg)
{
	perror(msg);
	exit(1);
}
int main(int argc, char *argv[])
{
	int sock,status,ret;
	char buf[100];
	struct hostent *host;
	struct sockaddr_in server;

	if (argc !=2)
	{
		printf("Client\n");
		exit(1);
	}
	if ((host = gethostbyname (argv[1])) == NULL)
		error("gethostname");
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 1)
		error("socket:");
	
	server.sin_family = AF_INET;
	server.sin_port = htons(3000);
	bzero(&server.sin_zero, 8);
	server.sin_addr = *((struct in_addr*)host->h_addr);

	if(connect(sock,(struct sockaddr *)&server, sizeof(struct sockaddr)) < 0)
		error("connect");

//pitfall 2
	while(1)
	{
		if(send(sock, "Holla bruh...", 14, 0) < 0)
			error("send");
		else
			printf("send success");
	}
	close(sock);
//pitfall 2
	status = read(sock, buf, sizeof(buf));
	if(status > 0)
		printf("Get data from socket");
	else if(status < 0)
		error("read");
	else if (status == 0)
	{
		printf("peer closed");
		close(sock);
	}

return 0;

}
