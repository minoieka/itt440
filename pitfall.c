#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<sys/wait.h>

void error(char *msg)
{
	perror(msg);
	exit(1);
}
int main(int argc, char *argv[])
{
	int sock,nsock,osock, n, i,status;
	struct sockaddr_in server, clilen;
	unsigned int len;
	char buf[300];

	osock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		error("socket : ");
	osock = sock;

	server.sin_family = AF_INET;
	server.sin_port = htons(3059);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 8);

	//pitfall 3
	int on, ret;
	on = 1;
	ret = setsockopt(sock, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(on));
	//pitfall 5
	ret = write(sock, "Message 1",50);
	ret = write(sock, "Message 2",50);
	
	len = sizeof(struct sockaddr_in);
	if((bind(sock, (struct sockaddr *)&server, len)) < 0)
		error("bind");
	if((listen (sock, 5)) < 0)
		error("listen");
	if((nsock=accept(sock, (struct sockaddr *)&clilen, &len)) <0)
		error("accept");

	fcntl(osock, F_SETFL, O_NONBLOCK);
	fcntl(nsock, F_SETFL, O_NONBLOCK);

	while(1)
	{
		for(i = sock; i <= osock; i++) {
			printf("Round num %d\n", i);
			if(i == sock) {
				len = sizeof(struct sockaddr_in);
				if((nsock = accept(sock, (struct sockaddr *)&clilen, &len)) < 0)
					error("accept");
				printf("server,connection from: %s\n", inet_ntoa(clilen.sin_addr));
			fcntl(nsock, F_SETFL, O_NONBLOCK);
			osock = nsock;
			}
			else {
				n = recv(nsock, buf, sizeof(buf),0);
				if(n < 1) {
					error("recv - non blocking \n");
					printf("String : %s \n",buf);
					if((send(nsock,"Hellow gais \n", 14,0)) < 1)
						error("send");
				}
			}
		}
	}
}
	/*create a UDP socket
	if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) ==-1)
	{
		die("socket");
	}
	//zero out the structure
	memset((char *) &si_me, 0, sizeof(si_me));
	
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	//bind socket to port
	if(bind(s, (struct sockaddr*)&si_me,sizeof(si_me)) == -1)
	{
		die("bind");
	}
	//keep listening for data
	while(1)
	{
		printf("waiting for data...");
		fflush(stdout);
		//try to received some data,this is a blocking call
		if((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
		{
			die("recvfrom()");
		}
		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Data: &s:%d\n", buf);
		//now reply the client with thesame data
		if(sendto(s, buf, recv_len,0,(struct sockaddr*) &si_other, slen) == -1)
		{
			die("sendto()");
		}

	}*/

/*int status, sock, mode;
	sock = socket(AF_INET,SOCK_DGRAM, 0);
	status = send(sock,buf,BUFLEN,MSG_DONTWAIT);
	if(status == -1)
	{
		printf("Send failed %s\n",strerror(errno));}
	else
	{
		printf("Success...!%s\n"); }

}




/*ret = read(sock, buffer, 1024);
ret = read(sock,buffer,1024);*/
